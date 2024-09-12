#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <string>

using boost::asio::ip::tcp;
namespace asio = boost::asio;


// 这个一个用于传输的数据
class dataSession : public std::enable_shared_from_this<dataSession> {
public:
	dataSession(tcp::socket socket, boost::asio::io_context& io_context) : socket_(std::move(socket)), io_context(io_context) {}
	void start() {

		do_read();
	}
private:
	tcp::socket socket_;  // 服务端socket
	boost::asio::io_context& io_context;  //类内引用，共享
	enum { max_length = 256 };
	std::array<char, max_length> data_; //这里假装使用字符串 const char* 

	void do_read() {
		auto self(shared_from_this()); // 共享指针
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length) {
				if (!ec) {
					std::cout << "Receive: " << std::string(data_.data(), length) << "\n";  //先看看不转化成字符串的效果: 屯屯屯。。。
					do_write(length);
				}
				else {
					std::cerr << "Read fail!" << ec.message() << "\n";
					socket_.close();
				}
			});
	}

	//在这里我想要写在终端上面
	void do_write(std::size_t lenth) {
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, lenth),
			[this, self](boost::system::error_code ec, std::size_t length) {
				if (!ec) {
					std::cout << "Send: " << std::string(data_.data(),length) << "\n";
					do_read();
				}
				else {
					std::cerr << "Write fail!" << ec.message() << "\n";
					std::cerr << ec.message() << "\n";
					socket_.close();
				}
			});
	}
};

// 服务端口
/**
* @brief 服务端口
* @note 这里面有：
*/
class Server {
public:
    // 破烂GPT,socket都不初始化
    Server(boost::asio::io_context& io_context, int port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), socket_(io_context) {
        start_accept();
    }
	tcp::socket socket_;  // 服务端socket


private:
    void start_accept() {
        // 然后这里不可可以使用共享指针创建套接字，这里在函数结束时自动释放
		// tcp::socket socket = tcp::socket(acceptor_.get_executor());  //get_io_context()已经不能用了，破烂GPT
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "Client Connected!" << "\n";
					// handle_connection(std::move(socket_)); 第二次就行不通了，有可能，所以需要转交socket所有权
					// tcp::socket socket = tcp::socket(acceptor_.get_executor()); //新的
					// 或者使用共享指针
					// std::make_shared<Session>(std::move(socket_), acceptor_.get_executor().context()) -> start();  //甚至可以在共享指针上面进行操作
					// 有点离谱，返回值还需要强制转化才可以，因为接受的是io_context& 的类型，但是返回的是executor_context 类型
					std::make_shared<dataSession>(std::move(socket_), static_cast<boost::asio::io_context&>(acceptor_.get_executor().context())) -> start();  //甚至可以在共享指针上面进行操作
                }
				else if(ec != boost::asio::error::operation_aborted)
				{
					std::cerr << "connect Fial!" << ec.message();
					
				}
				// 为下一个连接请求准备socket
				start_accept();
				// 现在我需要实现客户端数据的发送,这是一个无限循环，直接炸了
				// boost::system::error_code ec; //必须先创建一个操作
				// socket_.async_send(boost::asio::buffer("Hello",6), handler); 这个很古怪
				// socket_.write_some(boost::asio::buffer("Hello"), ec);
            });
    }

	void handler(
		const boost::system::error_code& error, // Result of operation.
		std::size_t bytes_transferred // Number of bytes sent.
	) 
	{
		//打印误差
		if (error) { std::cerr << "Send fail!" << error.message(); }
	};

	tcp::acceptor acceptor_;  // 服务端acceptor
};

//服务端试过之后我们尝试创建一个客户端，可以与服务端进行双向通信
class Client {
public:
	//同样需要一个io口服务，和一个服务端的ip地址和端口号
	//实现功能：连接到服务端，然后输出连接成功
    Client(boost::asio::io_service& io_service, const std::string& server, const std::string& port)  //端口有字符床
    : socket_(io_service){
		// 一连串的操作
		tcp::resolver resolver(io_service);   //解析器
		tcp::resolver::query query(server, port); //查询
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); //解析
		boost::asio::connect(socket_, endpoint_iterator);  //连接
		std::cout << "Connect to Service!!" << "\n";
    }

private:
	tcp::socket socket_;  // 客户端socket

 };


int main() {
 //   //然后可以简单的试一下
	//try {
	//	boost::asio::io_context io_context;
	//	Server server(io_context, 12345);
	//	io_context.run();
	//}
	//catch (std::exception& e) {
	//	std::cerr << e.what() << std::endl;
	//}
	//现在使用多线程实现客户端和服务端的通信,实际上并不是这么做
	try {
		boost::asio::io_context io_context;
		std::thread server_thread([&io_context]() {
			Server server(io_context, 12345);
			io_context.run();
			}); // lambada 函数极大简洁了
		//std::thread client_thread([&io_context]() {
		//	Client client(io_context, "localhost", "12345");
		//	io_context.run();
		//	});
		server_thread.join();
		//client_thread.join();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}


	return 0;
}


// 测试完毕，没有发现异常，到此简单的服务端和客户端的通信就建立了，但是还需要发送复杂的数据，需要进一步完善
// 先后关系： 客户端先连接，然后服务端口后接受消息
// 这代码好水，啥功能都没实现，明天继续改


// 致此，成功的实现了持续监听客户端
//接下来，我又需要去实现数据的收发，有时间继续写

//今天来实现数据的收发，可能需要大改变
//成功完成任务，多加一个dataSession,然后在dataSession里面实现数据的收发
//但是现在又出现了一个问题，就是发中文的时候会吞掉两个汉字,但在实际的数据传输中，这个问题不大

