//
// Created by 25030 on 2025/2/1.
//

#ifndef DA_LIU_REN_COMMON_HPP
#define DA_LIU_REN_COMMON_HPP

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

// 定义天干枚举，包含十个天干
enum class HeavenlyStem { Jia, Yi, Bing, Ding, Wu, Ji, Geng, Xin, Ren, Gui };
// 定义地支枚举，包含十二个地支
enum class EarthlyBranch { Zi, Chou, Yin, Mao, Chen, Si, Wu, Wei, Shen, You, Xu, Hai };

// 在 HeavenlyStem 枚举类型定义后添加以下代码
inline HeavenlyStem operator+(HeavenlyStem gan, int num) {
  int value = static_cast<int>(gan) + num;
  return static_cast<HeavenlyStem>(value % 10); // 假设有 10 个天干，取模确保在范围内
}

// 重载 operator+，支持 EarthlyBranch 类型的加法
inline EarthlyBranch operator+(EarthlyBranch dz, int num) {
  return static_cast<EarthlyBranch>((static_cast<int>(dz) + num + 12) % 12);
}

// 重载 operator++，支持 EarthlyBranch 类型的前置自增
inline EarthlyBranch &operator++(EarthlyBranch &dz) {
  dz = static_cast<EarthlyBranch>((static_cast<int>(dz) + 1) % 12);
  return dz;
}

// 重载 operator--，支持 EarthlyBranch 类型的前置自减
inline EarthlyBranch &operator--(EarthlyBranch &dz) {
  dz = static_cast<EarthlyBranch>((static_cast<int>(dz) - 1 + 12) % 12);
  return dz;
}

// 重载 operator-，支持 EarthlyBranch 类型的减法
inline int operator-(const EarthlyBranch &left, const EarthlyBranch &right) {
  return (static_cast<int>(left) - static_cast<int>(right) + 12) % 12;
}

// 天干地支名称映射
static std::map<HeavenlyStem, std::u8string> stemName = {
    {HeavenlyStem::Jia, u8"甲"},  {HeavenlyStem::Yi, u8"乙"},  {HeavenlyStem::Bing, u8"丙"},
    {HeavenlyStem::Ding, u8"丁"}, {HeavenlyStem::Wu, u8"戊"},  {HeavenlyStem::Ji, u8"己"},
    {HeavenlyStem::Geng, u8"庚"}, {HeavenlyStem::Xin, u8"辛"}, {HeavenlyStem::Ren, u8"壬"},
    {HeavenlyStem::Gui, u8"癸"}
};

static std::map<std::u8string, HeavenlyStem> stemMap = {
    {u8"甲", HeavenlyStem::Jia},  {u8"乙", HeavenlyStem::Yi},  {u8"丙", HeavenlyStem::Bing},
    {u8"丁", HeavenlyStem::Ding}, {u8"戊", HeavenlyStem::Wu},  {u8"己", HeavenlyStem::Ji},
    {u8"庚", HeavenlyStem::Geng}, {u8"辛", HeavenlyStem::Xin}, {u8"壬", HeavenlyStem::Ren},
    {u8"癸", HeavenlyStem::Gui}
};

static std::map<EarthlyBranch, std::u8string> branchName = {
    {EarthlyBranch::Zi, u8"子"},  {EarthlyBranch::Chou, u8"丑"}, {EarthlyBranch::Yin, u8"寅"},
    {EarthlyBranch::Mao, u8"卯"}, {EarthlyBranch::Chen, u8"辰"}, {EarthlyBranch::Si, u8"巳"},
    {EarthlyBranch::Wu, u8"午"},  {EarthlyBranch::Wei, u8"未"},  {EarthlyBranch::Shen, u8"申"},
    {EarthlyBranch::You, u8"酉"}, {EarthlyBranch::Xu, u8"戌"},   {EarthlyBranch::Hai, u8"亥"}
};

static std::map<std::u8string, EarthlyBranch> branchMap = {
    {u8"子", EarthlyBranch::Zi},  {u8"丑", EarthlyBranch::Chou}, {u8"寅", EarthlyBranch::Yin},
    {u8"卯", EarthlyBranch::Mao}, {u8"辰", EarthlyBranch::Chen}, {u8"巳", EarthlyBranch::Si},
    {u8"午", EarthlyBranch::Wu},  {u8"未", EarthlyBranch::Wei},  {u8"申", EarthlyBranch::Shen},
    {u8"酉", EarthlyBranch::You}, {u8"戌", EarthlyBranch::Xu},   {u8"亥", EarthlyBranch::Hai}
};

//地盘12个宫位数据
static std::vector<EarthlyBranch> earthPlateData = {
    EarthlyBranch::Zi, EarthlyBranch::Chou, EarthlyBranch::Yin,  EarthlyBranch::Mao, EarthlyBranch::Chen, EarthlyBranch::Si,
    EarthlyBranch::Wu, EarthlyBranch::Wei,  EarthlyBranch::Shen, EarthlyBranch::You, EarthlyBranch::Xu,   EarthlyBranch::Hai
};

// 天干五行，将每个天干映射到对应的五行数字
static std::map<HeavenlyStem, int> heavenlyStemFiveElements = {
    {HeavenlyStem::Jia, 1}, {HeavenlyStem::Yi, 1}, {HeavenlyStem::Bing, 2}, {HeavenlyStem::Ding, 2},
    {HeavenlyStem::Wu, 3},  {HeavenlyStem::Ji, 3}, {HeavenlyStem::Geng, 4}, {HeavenlyStem::Xin, 4},
    {HeavenlyStem::Ren, 5}, {HeavenlyStem::Gui, 5}
};

// 地支五行，将每个地支映射到对应的五行数字
static std::map<EarthlyBranch, int> earthlyBranchFiveElements = {
    {EarthlyBranch::Zi, 5},   {EarthlyBranch::Chou, 3}, {EarthlyBranch::Yin, 1}, {EarthlyBranch::Mao, 1},
    {EarthlyBranch::Chen, 3}, {EarthlyBranch::Si, 2},   {EarthlyBranch::Wu, 2},  {EarthlyBranch::Wei, 3},
    {EarthlyBranch::Shen, 4}, {EarthlyBranch::You, 4},  {EarthlyBranch::Xu, 3},  {EarthlyBranch::Hai, 5}
};

// 五行相生关系，判断 x 五行是否生 y 五行
inline bool generate(int x, int y) {
  return (x == 1 && y == 2) || (x == 2 && y == 3) || (x == 3 && y == 4) ||
         (x == 4 && y == 5) || (x == 5 && y == 1);
}

// 五行相克关系，判断 x 五行是否克 y 五行
inline bool overcome(int x, int y) {
  return (x == 1 && y == 3) || (x == 3 && y == 5) || (x == 5 && y == 2) ||
         (x == 2 && y == 4) || (x == 4 && y == 1);
}

// 天干阴阳属性，将每个天干映射到对应的阴阳属性（true 为阳，false 为阴）
static std::map<HeavenlyStem, bool> heavenlyStemYinYang = {
    {HeavenlyStem::Jia, true},   {HeavenlyStem::Yi, false},  {HeavenlyStem::Bing, true},
    {HeavenlyStem::Ding, false}, {HeavenlyStem::Wu, true},   {HeavenlyStem::Ji, false},
    {HeavenlyStem::Geng, true},  {HeavenlyStem::Xin, false}, {HeavenlyStem::Ren, true},
    {HeavenlyStem::Gui, false}
};

// 地支阴阳属性，将每个地支映射到对应的阴阳属性（true 为阳，false 为阴）
static std::map<EarthlyBranch, bool> earthlyBranchYinYang = {
    {EarthlyBranch::Zi, true},   {EarthlyBranch::Chou, false}, {EarthlyBranch::Yin, true},
    {EarthlyBranch::Mao, false}, {EarthlyBranch::Chen, true},  {EarthlyBranch::Si, false},
    {EarthlyBranch::Wu, true},   {EarthlyBranch::Wei, false},  {EarthlyBranch::Shen, true},
    {EarthlyBranch::You, false}, {EarthlyBranch::Xu, true},    {EarthlyBranch::Hai, false}
};

// 天干对应的阴阳贵人表
static std::map<HeavenlyStem, std::pair<EarthlyBranch, EarthlyBranch>> nobleTable = {
    {HeavenlyStem::Jia, {EarthlyBranch::Chou, EarthlyBranch::Wei}},
    {HeavenlyStem::Wu, {EarthlyBranch::Chou, EarthlyBranch::Wei}},
    {HeavenlyStem::Geng, {EarthlyBranch::Chou, EarthlyBranch::Wei}},
    {HeavenlyStem::Yi, {EarthlyBranch::Zi, EarthlyBranch::Shen}},
    {HeavenlyStem::Ji, {EarthlyBranch::Zi, EarthlyBranch::Shen}},
    {HeavenlyStem::Bing, {EarthlyBranch::Hai, EarthlyBranch::You}},
    {HeavenlyStem::Ding, {EarthlyBranch::Hai, EarthlyBranch::You}},
    {HeavenlyStem::Ren, {EarthlyBranch::Si, EarthlyBranch::Mao}},
    {HeavenlyStem::Gui, {EarthlyBranch::Si, EarthlyBranch::Mao}},
    {HeavenlyStem::Xin, {EarthlyBranch::Wu, EarthlyBranch::Yin}}
};

// 天干寄宫表
static std::map<HeavenlyStem, EarthlyBranch> palaceTable = {
    {HeavenlyStem::Jia, EarthlyBranch::Yin},   // 甲寄寅
    {HeavenlyStem::Yi, EarthlyBranch::Chen},   // 乙寄辰
    {HeavenlyStem::Bing, EarthlyBranch::Si},   // 丙寄巳
    {HeavenlyStem::Ding, EarthlyBranch::Wei},  // 丁寄未
    {HeavenlyStem::Wu, EarthlyBranch::Si},     // 戊寄巳
    {HeavenlyStem::Ji, EarthlyBranch::Wei},    // 己寄未
    {HeavenlyStem::Geng, EarthlyBranch::Shen}, // 庚寄申
    {HeavenlyStem::Xin, EarthlyBranch::Xu},    // 辛寄戌
    {HeavenlyStem::Ren, EarthlyBranch::Hai},   // 壬寄亥
    {HeavenlyStem::Gui, EarthlyBranch::Chou}   // 癸寄丑
};

// 十二神将名称
static std::vector<std::u8string> divineGenerals = {u8"贵人", u8"螣蛇", u8"朱雀", u8"六合",
                                                   u8"勾陈", u8"青龙", u8"天空", u8"白虎",
                                                   u8"太常", u8"玄武", u8"太阴", u8"天后"};

// 地支名称
static std::vector<std::u8string> earthlyBranchNames = {u8"子", u8"丑", u8"寅", u8"卯", u8"辰", u8"巳",
                                                       u8"午", u8"未", u8"申", u8"酉", u8"戌", u8"亥"};


// 判断两个天干的阴阳属性是否相同
inline bool yinYangSame(HeavenlyStem stem1, HeavenlyStem stem2) {
  return heavenlyStemYinYang[stem1] == heavenlyStemYinYang[stem2];
}

// 辅助函数，根据地支获取对应的天干寄宫
inline std::vector<HeavenlyStem> getHeavenlyStemsOfPalace(EarthlyBranch branch) {
  std::vector<HeavenlyStem> result;
  switch (branch) {
  case EarthlyBranch::Yin:
    // 甲寄寅
    result.push_back(HeavenlyStem::Jia);
    break;
  case EarthlyBranch::Chen:
    // 乙寄辰
    result.push_back(HeavenlyStem::Yi);
    break;
  case EarthlyBranch::Si:
    // 丙、戊寄巳
    result.push_back(HeavenlyStem::Bing);
    result.push_back(HeavenlyStem::Wu);
    break;
  case EarthlyBranch::Wei:
    // 丁、己寄未
    result.push_back(HeavenlyStem::Ding);
    result.push_back(HeavenlyStem::Ji);
    break;
  case EarthlyBranch::Shen:
    // 庚寄申
    result.push_back(HeavenlyStem::Geng);
    break;
  case EarthlyBranch::Xu:
    // 辛寄戌
    result.push_back(HeavenlyStem::Xin);
    break;
  case EarthlyBranch::Hai:
    // 壬寄亥
    result.push_back(HeavenlyStem::Ren);
    break;
  case EarthlyBranch::Chou:
    // 癸寄丑
    result.push_back(HeavenlyStem::Gui);
    break;
  default:
    // 子、卯、午、酉无天干寄宫
    break;
  }
  return result;
}

// 根据天干获取对应的天干寄宫地支
inline EarthlyBranch getPalace(HeavenlyStem stem) {
  // 天干寄宫逻辑
  switch (stem) {
  case HeavenlyStem::Jia:
    return EarthlyBranch::Yin;
  case HeavenlyStem::Yi:
    return EarthlyBranch::Chen;
  case HeavenlyStem::Bing:
    return EarthlyBranch::Si;
  case HeavenlyStem::Ding:
    return EarthlyBranch::Wei;
  case HeavenlyStem::Wu:
    return EarthlyBranch::Si;
  case HeavenlyStem::Ji:
    return EarthlyBranch::Wei;
  case HeavenlyStem::Geng:
    return EarthlyBranch::Shen;
  case HeavenlyStem::Xin:
    return EarthlyBranch::Xu;
  case HeavenlyStem::Ren:
    return EarthlyBranch::Hai;
  case HeavenlyStem::Gui:
    return EarthlyBranch::Chou;
  default:
    return EarthlyBranch::Zi;
  }
}

// 根据地支获取其阴神
inline EarthlyBranch getBranchYinGod(EarthlyBranch branch) {
  // 获取地支阴神逻辑
  int index = static_cast<int>(branch);
  return static_cast<EarthlyBranch>((index + 6) % 12);
}

// 判断两个地支是否相刑
inline bool conflict(EarthlyBranch branch1, EarthlyBranch branch2) {
  // 地支相刑逻辑简化
  // 地支相刑逻辑
  const std::map<EarthlyBranch, EarthlyBranch> conflictMap = {
      {EarthlyBranch::Zi, EarthlyBranch::Mao},    {EarthlyBranch::Chou, EarthlyBranch::Xu},
      {EarthlyBranch::Yin, EarthlyBranch::Si},    {EarthlyBranch::Mao, EarthlyBranch::Zi},
      {EarthlyBranch::Chen, EarthlyBranch::Chen}, {EarthlyBranch::Si, EarthlyBranch::Shen},
      {EarthlyBranch::Wu, EarthlyBranch::Wu},     {EarthlyBranch::Wei, EarthlyBranch::Chou},
      {EarthlyBranch::Shen, EarthlyBranch::Yin},  {EarthlyBranch::You, EarthlyBranch::You},
      {EarthlyBranch::Xu, EarthlyBranch::Wei},    {EarthlyBranch::Hai, EarthlyBranch::Hai}
  };

  auto it = conflictMap.find(branch1);
  if (it!= conflictMap.end()) {
    return it->second == branch2;
  }
  return false;
}

// 判断地支是否为寅、申、巳、亥
inline bool isYinShenSiHai(EarthlyBranch branch) {
  return branch == EarthlyBranch::Yin || branch == EarthlyBranch::Shen || branch == EarthlyBranch::Si ||
         branch == EarthlyBranch::Hai;
}

// 判断两个地支是否相冲
inline bool oppose(EarthlyBranch branch1, EarthlyBranch branch2) {
  return (static_cast<int>(branch1) + 6) % 12 == static_cast<int>(branch2);
}

// 判断昼夜（卯到申为昼）
inline bool isDaytime(EarthlyBranch hour) {
  int idx = static_cast<int>(hour);
  return (idx >= 3 && idx <= 8); // 3=卯, 8=申
}

// 根据天干和昼夜获取贵人所在地支
inline EarthlyBranch getNoble(HeavenlyStem stem, bool isDay) {
  auto &noblePair = nobleTable[stem];
  return isDay ? noblePair.first : noblePair.second;
}

// 获取月将的函数
inline EarthlyBranch getMoonGeneral(const LunarObj *obj) {
  const std::map<int, EarthlyBranch> moonGeneralTable = {
      {1, EarthlyBranch::Hai},   // 正月（寅） - 登明（亥）
      {2, EarthlyBranch::Xu},    // 二月（卯） - 河魁（戌）
      {3, EarthlyBranch::You},   // 三月（辰） - 传送（酉）
      {4, EarthlyBranch::Shen},  // 四月（巳） - 小吉（申）
      {5, EarthlyBranch::Wei},   // 五月（午） - 胜光（未）
      {6, EarthlyBranch::Si},    // 六月（未） - 太乙（午）
      {7, EarthlyBranch::Wu},    // 七月（申） - 天罡（巳）
      {8, EarthlyBranch::Chen},  // 八月（酉） - 太冲（辰）
      {9, EarthlyBranch::Mao},   // 九月（戌） - 功曹（卯）
      {10, EarthlyBranch::Yin},  // 十月（亥） - 大吉（寅）
      {11, EarthlyBranch::Chou}, // 十一月（子） - 神后（丑）
      {12, EarthlyBranch::Zi}    // 十二月（丑） - 阴阳（子）
  };

  return moonGeneralTable.at(obj->lunarMonth);
}

#endif // DA_LIU_REN_COMMON_HPP
