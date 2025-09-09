#include "game_manager.h"

int main() {
    try {
        GameManager game;
        game.Run();
        std::cout << "感谢游玩！" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "游戏异常: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "未知错误。" << std::endl;
        return 1;
    }
    return 0;
}