#include "game_manager.h"

int main() {
    try {
        GameManager game;
        game.Run();
        std::cout << "��л���棡" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "��Ϸ�쳣: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "δ֪����" << std::endl;
        return 1;
    }
    return 0;
}