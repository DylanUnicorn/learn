#include <FormatObject.h>
#include <vector>


// Example of a struct inheriting from FormattableObject
struct MyStruct : public Cango::TaskDesign::FormattableObject {
    Cango::TaskDesign::FormattableVariant<int, double, std::string> data;

    void Format(fmt::memory_buffer& buffer) const noexcept override {
        data.Format(buffer);
    }
};

int main() {
    // Test single object formatting
    MyStruct s;
    s.data = 42;

    fmt::print("Single object: {}\n", s);

    return 0;
}