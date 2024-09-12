#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <string>

using boost::asio::ip::tcp;
namespace asio = boost::asio;


// ���һ�����ڴ��������
class dataSession : public std::enable_shared_from_this<dataSession> {
public:
	dataSession(tcp::socket socket, boost::asio::io_context& io_context) : socket_(std::move(socket)), io_context(io_context) {}
	void start() {

		do_read();
	}
private:
	tcp::socket socket_;  // �����socket
	boost::asio::io_context& io_context;  //�������ã�����
	enum { max_length = 256 };
	std::array<char, max_length> data_; //�����װʹ���ַ��� const char* 

	void do_read() {
		auto self(shared_from_this()); // ����ָ��
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length) {
				if (!ec) {
					std::cout << "Receive: " << std::string(data_.data(), length) << "\n";  //�ȿ�����ת�����ַ�����Ч��: �����͡�����
					do_write(length);
				}
				else {
					std::cerr << "Read fail!" << ec.message() << "\n";
					socket_.close();
				}
			});
	}

	//����������Ҫд���ն�����
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

// ����˿�
/**
* @brief ����˿�
* @note �������У�
*/
class Server {
public:
    // ����GPT,socket������ʼ��
    Server(boost::asio::io_context& io_context, int port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), socket_(io_context) {
        start_accept();
    }
	tcp::socket socket_;  // �����socket


private:
    void start_accept() {
        // Ȼ�����ﲻ�ɿ���ʹ�ù���ָ�봴���׽��֣������ں�������ʱ�Զ��ͷ�
		// tcp::socket socket = tcp::socket(acceptor_.get_executor());  //get_io_context()�Ѿ��������ˣ�����GPT
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "Client Connected!" << "\n";
					// handle_connection(std::move(socket_)); �ڶ��ξ��в�ͨ�ˣ��п��ܣ�������Ҫת��socket����Ȩ
					// tcp::socket socket = tcp::socket(acceptor_.get_executor()); //�µ�
					// ����ʹ�ù���ָ��
					// std::make_shared<Session>(std::move(socket_), acceptor_.get_executor().context()) -> start();  //���������ڹ���ָ��������в���
					// �е����ף�����ֵ����Ҫǿ��ת���ſ��ԣ���Ϊ���ܵ���io_context& �����ͣ����Ƿ��ص���executor_context ����
					std::make_shared<dataSession>(std::move(socket_), static_cast<boost::asio::io_context&>(acceptor_.get_executor().context())) -> start();  //���������ڹ���ָ��������в���
                }
				else if(ec != boost::asio::error::operation_aborted)
				{
					std::cerr << "connect Fial!" << ec.message();
					
				}
				// Ϊ��һ����������׼��socket
				start_accept();
				// ��������Ҫʵ�ֿͻ������ݵķ���,����һ������ѭ����ֱ��ը��
				// boost::system::error_code ec; //�����ȴ���һ������
				// socket_.async_send(boost::asio::buffer("Hello",6), handler); ����ܹŹ�
				// socket_.write_some(boost::asio::buffer("Hello"), ec);
            });
    }

	void handler(
		const boost::system::error_code& error, // Result of operation.
		std::size_t bytes_transferred // Number of bytes sent.
	) 
	{
		//��ӡ���
		if (error) { std::cerr << "Send fail!" << error.message(); }
	};

	tcp::acceptor acceptor_;  // �����acceptor
};

//������Թ�֮�����ǳ��Դ���һ���ͻ��ˣ����������˽���˫��ͨ��
class Client {
public:
	//ͬ����Ҫһ��io�ڷ��񣬺�һ������˵�ip��ַ�Ͷ˿ں�
	//ʵ�ֹ��ܣ����ӵ�����ˣ�Ȼ��������ӳɹ�
    Client(boost::asio::io_service& io_service, const std::string& server, const std::string& port)  //�˿����ַ���
    : socket_(io_service){
		// һ�����Ĳ���
		tcp::resolver resolver(io_service);   //������
		tcp::resolver::query query(server, port); //��ѯ
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); //����
		boost::asio::connect(socket_, endpoint_iterator);  //����
		std::cout << "Connect to Service!!" << "\n";
    }

private:
	tcp::socket socket_;  // �ͻ���socket

 };


int main() {
 //   //Ȼ����Լ򵥵���һ��
	//try {
	//	boost::asio::io_context io_context;
	//	Server server(io_context, 12345);
	//	io_context.run();
	//}
	//catch (std::exception& e) {
	//	std::cerr << e.what() << std::endl;
	//}
	//����ʹ�ö��߳�ʵ�ֿͻ��˺ͷ���˵�ͨ��,ʵ���ϲ�������ô��
	try {
		boost::asio::io_context io_context;
		std::thread server_thread([&io_context]() {
			Server server(io_context, 12345);
			io_context.run();
			}); // lambada ������������
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


// ������ϣ�û�з����쳣�����˼򵥵ķ���˺Ϳͻ��˵�ͨ�žͽ����ˣ����ǻ���Ҫ���͸��ӵ����ݣ���Ҫ��һ������
// �Ⱥ��ϵ�� �ͻ��������ӣ�Ȼ�����˿ں������Ϣ
// ������ˮ��ɶ���ܶ�ûʵ�֣����������


// �´ˣ��ɹ���ʵ���˳��������ͻ���
//��������������Ҫȥʵ�����ݵ��շ�����ʱ�����д

//������ʵ�����ݵ��շ���������Ҫ��ı�
//�ɹ�������񣬶��һ��dataSession,Ȼ����dataSession����ʵ�����ݵ��շ�
//���������ֳ�����һ�����⣬���Ƿ����ĵ�ʱ����̵���������,����ʵ�ʵ����ݴ����У�������ⲻ��

