#ifndef DA_LIU_REN_LIU_REN_HPP
#define DA_LIU_REN_LIU_REN_HPP

#include "Lunar.h" // 引入农历库头文件
#include "common.hpp"
#include <algorithm>
#include <cmath>
#include <codecvt>
#include <format>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

// 干支类，用于表示一个干支组合
class StemBranch {
public:
  union {
    HeavenlyStem stem;
    EarthlyBranch lowerBranch;
  };
  EarthlyBranch branch;
  bool isFirstOvercome;

  StemBranch(HeavenlyStem s, EarthlyBranch b)
      : stem(s), branch(b), isFirstOvercome(true) {}
  StemBranch(EarthlyBranch lower, EarthlyBranch upper)
      : lowerBranch(lower), branch(upper), isFirstOvercome(false) {}

  int getFiveElements() const {
    return isFirstOvercome ? heavenlyStemFiveElements.at(stem)
                           : earthlyBranchFiveElements.at(lowerBranch);
  }

  bool isYang() const {
    return isFirstOvercome ? heavenlyStemYinYang.at(stem)
                           : earthlyBranchYinYang.at(lowerBranch);
  }

  bool operator==(const StemBranch &other) const {
    if (isFirstOvercome != other.isFirstOvercome)
      return false;
    if (isFirstOvercome)
      return stem == other.stem && branch == other.branch;
    return lowerBranch == other.lowerBranch && branch == other.branch;
  }
};

// 四课类，用于表示四课信息
class FourLessons {
public:
  StemBranch firstLesson;      // 第一课
  StemBranch secondLesson;     // 第二课
  StemBranch thirdLesson;      // 第三课
  StemBranch fourthLesson;     // 第四课
  EarthlyBranch stemYangGod;   // 干阳神
  EarthlyBranch branchYangGod; // 支阳神

  FourLessons(const StemBranch &fl, const StemBranch &sl, const StemBranch &tl,
              const StemBranch &frl, EarthlyBranch sYG, EarthlyBranch bYG)
      : firstLesson(fl), secondLesson(sl), thirdLesson(tl), fourthLesson(frl),
        stemYangGod(sYG), branchYangGod(bYG) {}
};

// 天地盘类，包含地盘、天盘、十二神将、十二宫位对应的神煞列表和天将盘信息
class HeavenEarthPlate {
public:
  std::vector<EarthlyBranch> earthPlate;     // 地盘地支数组
  std::vector<EarthlyBranch> heavenPlate;    // 天盘地支数组
  std::vector<EarthlyBranch> divineGenerals; // 十二神将位置
  std::map<EarthlyBranch, std::vector<std::string>> shenShaTable; // 神煞表

  HeavenEarthPlate(const std::vector<EarthlyBranch> &ep,
                   const std::vector<EarthlyBranch> &hp,
                   const std::vector<EarthlyBranch> &dg, HeavenlyStem stem,
                   bool isDay, const LunarObj *obj)
      : earthPlate(ep), heavenPlate(hp), divineGenerals(dg) {
    // 获取贵人所在地支
    auto &noblePair = nobleTable[stem];
    EarthlyBranch noble = isDay ? noblePair.first : noblePair.second;
    int nobleIndex = static_cast<int>(noble);

    // 初始化神煞表
    initializeShenShaTable(obj);
  }

  // 重载 [] 运算符，根据地支获取天盘上对应的地支
  EarthlyBranch operator[](EarthlyBranch branch) const {
    int index = static_cast<int>(branch);
    return heavenPlate[index];
  }

  // 获取十二神将的位置
  EarthlyBranch getDivineGeneral(EarthlyBranch branch) const {
    int index = static_cast<int>(branch);
    return divineGenerals[index];
  }

  // 根据地支获取神煞列表
  std::vector<std::string> getShenSha(EarthlyBranch branch) const {
    return shenShaTable.at(branch);
  }

  // 格式化输出天、地盘12宫的信息，以及十二神将位置和神煞表
  void printPlateInfo() const {

    // 输出地盘信息
    fmt::print("地盘信息: ");
    for (const auto &branch : earthPlate) {
      // 转换为 std::string
      std::string branchNameStr(branchName.at(branch).begin(), branchName.at(branch).end());
      fmt::print("{}", branchNameStr);
      if (&branch != &earthPlate.back()) {
        fmt::print(" ");
      }
    }
    fmt::print("\n");

    // 输出天盘信息
    fmt::print("天盘信息: ");
    for (const auto &branch : heavenPlate) {
      // 转换为 std::string
      std::string branchNameStr(branchName.at(branch).begin(), branchName.at(branch).end());
      fmt::print("{}", branchNameStr);
      if (&branch != &heavenPlate.back()) {
        fmt::print(" ");
      }
    }
    fmt::print("\n");

    // 输出神煞表信息
    fmt::print("神煞表信息:\n");
    for (const auto &[branch, shaList] : shenShaTable) {
      // 转换为 std::string
      std::string branchNameStr(branchName.at(branch).begin(), branchName.at(branch).end());
      fmt::print("地支: {} 神煞: ", branchNameStr);
      fmt::print("{}", fmt::join(shaList, " "));
      fmt::print("\n");
    }
  }

private:
  // 初始化神煞表，根据大六壬排盘规则动态生成
  void initializeShenShaTable(const LunarObj *obj) {
    // 获取当前年份的地支（太岁）
    HeavenlyStem currentYearStem;
    EarthlyBranch currentYearBranch;
    std::u8string ganzhiYear(obj->ganzhiYear.begin(), obj->ganzhiYear.end());
    std::u8string yearStr = ganzhiYear.substr(0, 3);
    currentYearStem = stemMap[yearStr];
    currentYearBranch = branchMap[ganzhiYear.substr(3, 5)];
  }
};

// 排列十二神将
inline std::vector<EarthlyBranch>
arrangeDivineGenerals(EarthlyBranch nobleBranch, bool isClockwise) {
  std::vector<EarthlyBranch> divineGeneralPositions(12);
  int nobleIndex = static_cast<int>(nobleBranch);
  int step = isClockwise ? 1 : -1;
  for (int i = 0; i < 12; ++i) {
    int pos = (nobleIndex + i * step + 12) % 12;
    divineGeneralPositions[i] = static_cast<EarthlyBranch>(pos);
  }
  return divineGeneralPositions;
}

// 排列天盘
inline std::vector<EarthlyBranch>
arrangeHeavenPlate(EarthlyBranch moonGeneral) {
  std::vector<EarthlyBranch> heavenPlateData(12);
  EarthlyBranch current = moonGeneral;
  for (int i = 0; i < 12; ++i) {
    heavenPlateData[i] = current;
    current = static_cast<EarthlyBranch>((static_cast<int>(current) + 1) %
                                         12); // 顺时针排列
  }
  return heavenPlateData;
}

// 三传类，用于计算和表示三传信息
class ThreeTransmissions {
private:
  const HeavenEarthPlate &heavenEarthPlate; // 引用天地盘对象
  const FourLessons &fourLessons;           // 引用四课对象
  EarthlyBranch initial;                    // 初传
  EarthlyBranch middle;                     // 中传
  EarthlyBranch finalTransmission;          // 末传
  std::vector<std::u8string> pattern;       // 三传的格局类型

  // 去除重复课的辅助函数，避免相同天干的课重复出现
  std::vector<StemBranch>
  removeDuplicateLessons(const std::vector<StemBranch> &lessons) const;

  // 查找有贼克的课，即地支五行克天干五行的课
  std::vector<StemBranch> haveConquerors() const;

  // 查找有克的课，即天干五行克地支五行的课
  std::vector<StemBranch> haveOvercomes() const;

  // 贼克法取三传
  std::vector<EarthlyBranch> thiefConquer();

  // 比用法取三传
  std::vector<EarthlyBranch>
  comparisonUse(const std::vector<StemBranch> &lessons);

  // 涉害法取三传
  std::vector<EarthlyBranch>
  harmInvolved(const std::vector<StemBranch> &lessonsVec);

  // 遥克法取三传
  std::vector<EarthlyBranch> remoteOvercome();

  // 昂星法取三传
  std::vector<EarthlyBranch> angStar();

  // 别责法取三传
  std::vector<EarthlyBranch> specialResponsibility();

  // 八专法取三传
  std::vector<EarthlyBranch> eightSpecial();

  // 伏吟法取三传
  std::vector<EarthlyBranch> staticChant();

  // 返吟法取三传
  std::vector<EarthlyBranch> reverseChant();

  // 判断地支是否为孟神（寅、巳、申、亥）
  bool isMeng(EarthlyBranch branch) const;

  // 判断地支是否为仲神（子、卯、午、酉）
  bool isZhong(EarthlyBranch branch) const;

  // 判断干支是否为八传日
  bool isEightTransmissionDay(const StemBranch &stemBranch) const;

  // 判断是否为伏吟课
  bool isStaticChantLesson() const;

  // 判断是否为返吟课
  bool isReverseChantLesson() const;

public:
  // 三传类构造函数，根据四课和天地盘信息计算三传
  ThreeTransmissions(const HeavenEarthPlate &he, const FourLessons &s);

  // 获取初传地支
  EarthlyBranch getInitial() const;
  // 获取中传地支
  EarthlyBranch getMiddle() const;
  // 获取末传地支
  EarthlyBranch getFinalTransmission() const;
  // 获取三传的格局类型
  const std::vector<std::u8string> &getPattern() const;
};

inline int test01() {
  // ---- 输入参数 ----
  int year, month, day, hour;
  std::println(std::cout, "请输入阳历日期（年 月 日 时）：");
  std::cin >> year >> month >> day >> hour;

  // ---- Step 1: 使用农历库获取农历信息 ----
  Lunar lunar;
  LunarObj *obj = lunar.solar2lunar(year, month, day);

  std::println(std::cout, "农历日期：{}年{}{}\n", obj->lunarYear,
               obj->lunarMonthChineseName, obj->lunarDayChineseName);
  std::println(std::cout, "生肖：{}\n", obj->animal);
  std::println(std::cout, "干支年：{}\n", obj->ganzhiYear);
  std::println(std::cout, "干支月：{}\n", obj->ganzhiMonth);
  std::println(std::cout, "干支日：{}\n", obj->ganzhiDay);
  std::println(std::cout, "节气：{}\n", obj->term);

  // ---- Step 2: 更优雅地提取日干和日支 ----
  std::u8string ganzhiDayU8 =
      std::u8string(obj->ganzhiDay.begin(), obj->ganzhiDay.end());

  if (ganzhiDayU8.length() != 6) { // 假设每个汉字UTF-8编码为3字节，总共6字节
    throw std::runtime_error(
        "干支日字符串长度异常，预期为两个汉字 (6字节 UTF-8)");
  }

  std::u8string stemU8Str = ganzhiDayU8.substr(0, 3);   // 截取前3个字节作为干
  std::u8string branchU8Str = ganzhiDayU8.substr(3, 3); // 截取后3个字节作为支

  HeavenlyStem dayStem;
  EarthlyBranch dayBranch;

  try {
    dayStem = stemMap.at(stemU8Str);       // 日干
    dayBranch = branchMap.at(branchU8Str); // 日支
  } catch (const std::out_of_range &e) {
    std::println(std::cerr, "Error: 干支字符无法识别: {}\n", e.what());
    throw; // 重新抛出异常，或者进行其他错误处理
  }

  EarthlyBranch timePeriod =
      static_cast<EarthlyBranch>((hour + 1) / 2 % 12); // 时辰计算
  EarthlyBranch currentHour = timePeriod;              // 当前时辰用于判断昼夜

  // ---- Step 3: 确定贵人 ----
  bool isDay = isDaytime(currentHour);

  // ---- Step 4: 排列十二神将 ----
  EarthlyBranch nobleBranch = getNoble(dayStem, isDay);
  bool isClockwise =
      (nobleBranch == EarthlyBranch::Hai) ||
      (static_cast<int>(nobleBranch) >= static_cast<int>(EarthlyBranch::Zi) &&
       static_cast<int>(nobleBranch) <= static_cast<int>(EarthlyBranch::Chen));

  std::vector<EarthlyBranch> divineGeneralPositions =
      arrangeDivineGenerals(nobleBranch, isClockwise);

  // ---- Step 5: 获取月将 ----
  EarthlyBranch moonGeneral = getMoonGeneral(obj);

  // ---- Step 6: 初始化天盘 ----
  std::vector<EarthlyBranch> heavenPlateData = arrangeHeavenPlate(moonGeneral);

  // ---- Step 7: 创建天地盘对象 ----
  HeavenEarthPlate heavenEarthPlate(earthPlateData, heavenPlateData,
                                    divineGeneralPositions, dayStem, isDay,
                                    obj);

  // ---- Step 8: 计算四课 ----
  // 第一课：干上神
  EarthlyBranch dayStemPalace = getPalace(dayStem);
  EarthlyBranch firstLessonUpperGod = heavenEarthPlate[dayStemPalace];
  StemBranch firstLesson(dayStem, firstLessonUpperGod);

  // 第二课：第一课上神在天盘对应位置
  EarthlyBranch secondLessonUpperGod = heavenEarthPlate[firstLessonUpperGod];
  StemBranch secondLesson(firstLessonUpperGod, secondLessonUpperGod);

  // 第三课：支上神
  EarthlyBranch thirdLessonUpperGod = heavenEarthPlate[dayBranch];
  StemBranch thirdLesson(dayBranch, thirdLessonUpperGod);

  // 第四课：第三课上神在天盘对应位置
  EarthlyBranch fourthLessonUpperGod = heavenEarthPlate[thirdLessonUpperGod];
  StemBranch fourthLesson(thirdLessonUpperGod, fourthLessonUpperGod);

  // 干阳神地支和支阳神地支
  EarthlyBranch stemYangGodBranch = firstLessonUpperGod;
  EarthlyBranch branchYangGodBranch = thirdLessonUpperGod;

  // 创建四课对象
  FourLessons fourLessonsObj(firstLesson, secondLesson, thirdLesson,
                             fourthLesson, stemYangGodBranch,
                             branchYangGodBranch);
  // 创建三传对象
  ThreeTransmissions threeTransmissions(heavenEarthPlate, fourLessonsObj);

  // 输出初传地支编号
  std::cout << std::format("初传: {}\n", std::string(branchName[threeTransmissions.getInitial()].begin(), branchName[threeTransmissions.getInitial()].end())) << std::endl; // Convert to std::string for formatting
  // 输出中传地支编号
  std::cout << std::format("中传: {}\n", std::string(branchName[threeTransmissions.getMiddle()].begin(), branchName[threeTransmissions.getMiddle()].end())) << std::endl; // Convert to std::string for formatting
  // 输出末传地支编号
  std::cout << std::format("末传: {}\n", std::string(branchName[threeTransmissions.getFinalTransmission()].begin(), branchName[threeTransmissions.getFinalTransmission()].end())) << std::endl; // Convert to std::string for formatting
  // 输出三传的格局类型
  for (const auto &p : threeTransmissions.getPattern()) {
    std::cout << std::format("格局: {}\n", std::string(p.begin(), p.end())) << std::endl; // Convert to std::string for formatting
  }


  heavenEarthPlate.printPlateInfo();

  return 0;
}
#endif // DA_LIU_REN_LIU_REN_HPP