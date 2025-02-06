
#include "liu_ren.hpp"

using namespace std;



int main() {
  // 设置本地化环境
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale());
  test01();

}