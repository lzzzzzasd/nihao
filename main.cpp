#include "game_manager.h"

int main() {
    try {
        GameManager game;
        game.Run();
        std::cout << "¸ĞĞ»ÓÎÍæMUDÃ°ÏÕ£¡" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "ÓÎÏ·Òì³£: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Î´Öª´íÎó¡£" << std::endl;
        return 1;
    }
    return 0;
}