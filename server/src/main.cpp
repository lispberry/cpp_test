#include <server.hpp>

int main() {
    server::FileServer server(7777);
    server.start(std::filesystem::current_path() / "test.txt");
    server.wait();

    return 0;
}