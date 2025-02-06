#include "liu_ren.hpp"

// 去除重复课的辅助函数，避免相同天干的课重复出现
std::vector<StemBranch>
ThreeTransmissions::removeDuplicateLessons(const std::vector<StemBranch> &lessons) const {
  std::vector<StemBranch> lessonsTmp;
  std::vector<HeavenlyStem> listTmp;
  for (const auto &l : lessons) {
    if (std::find(listTmp.begin(), listTmp.end(), l.stem) == listTmp.end()) {
      listTmp.push_back(l.stem);
      lessonsTmp.push_back(l);
    }
  }
  return lessonsTmp;
}

// 查找有贼克的课，即地支五行克天干五行的课
std::vector<StemBranch> ThreeTransmissions::haveConquerors() const {
  std::vector<StemBranch> lessonsVec;
  const std::vector<StemBranch> lessons = {
      fourLessons.firstLesson, fourLessons.secondLesson,
      fourLessons.thirdLesson, fourLessons.fourthLesson};
  for (const auto &l : lessons) {
    if (overcome(l.getFiveElements(), heavenlyStemFiveElements[l.stem])) {
      lessonsVec.push_back(l);
    }
  }
  return removeDuplicateLessons(lessonsVec);
}

// 查找有克的课，即天干五行克地支五行的课
std::vector<StemBranch> ThreeTransmissions::haveOvercomes() const {
  std::vector<StemBranch> lessonsVec;
  const std::vector<StemBranch> lessons = {
      fourLessons.firstLesson, fourLessons.secondLesson,
      fourLessons.thirdLesson, fourLessons.fourthLesson};
  for (const auto &l : lessons) {
    if (overcome(heavenlyStemFiveElements[l.stem], l.getFiveElements())) {
      lessonsVec.push_back(l);
    }
  }
  return removeDuplicateLessons(lessonsVec);
}

// 贼克法取三传
std::vector<EarthlyBranch> ThreeTransmissions::thiefConquer() {
  auto conquerors = haveConquerors();
  if (!conquerors.empty()) {
    if (conquerors.size() == 1) {
      // 若只有一个贼克课，该课的地支为初传
      initial = conquerors[0].branch;
      // 初传对应的天盘地支为中传
      middle = heavenEarthPlate[initial];
      // 中传对应的天盘地支为末传
      finalTransmission = heavenEarthPlate[middle];
      pattern.push_back(u8"重审卦");
      return {initial, middle, finalTransmission};
    } else {
      // 多个贼克课，进入比用法
      return comparisonUse(conquerors);
    }
  }
  auto overcomes = haveOvercomes();
  if (!overcomes.empty()) {
    if (overcomes.size() == 1) {
      // 若只有一个克课，该课的地支为初传
      initial = overcomes[0].branch;
      // 初传对应的天盘地支为中传
      middle = heavenEarthPlate[initial];
      // 中传对应的天盘地支为末传
      finalTransmission = heavenEarthPlate[middle];
      pattern.push_back(u8"元首卦");
      return {initial, middle, finalTransmission};
    } else {
      // 多个克课，进入比用法
      return comparisonUse(overcomes);
    }
  }
  // 没有贼克和克课，抛出异常
  throw std::runtime_error("不能用贼克取三传");
}

// 比用法取三传
std::vector<EarthlyBranch>
ThreeTransmissions::comparisonUse(const std::vector<StemBranch> &lessons) {
  std::vector<StemBranch> result;
  for (const auto &l : lessons) {
    if (yinYangSame(l.stem, fourLessons.firstLesson.stem)) {
      result.push_back(l);
    }
  }
  if (result.size() == 1) {
    // 若只有一个符合比用条件的课，该课的地支为初传
    initial = result[0].branch;
    // 初传对应的天盘地支为中传
    middle = heavenEarthPlate[initial];
    // 中传对应的天盘地支为末传
    finalTransmission = heavenEarthPlate[middle];
    pattern.push_back(u8"知一卦");
    return {initial, middle, finalTransmission};
  } else if (result.size() == 0) {
    // 没有符合比用条件的课，进入涉害法
    return harmInvolved(lessons);
  } else {
    // 多个符合比用条件的课，进入涉害法
    return harmInvolved(result);
  }
}

// 涉害法取三传
std::vector<EarthlyBranch>
ThreeTransmissions::harmInvolved(const std::vector<StemBranch> &lessonsVec) {
  std::vector<std::pair<StemBranch, int>> lessonsHarmDepth;
  for (const auto &l : lessonsVec) {
    EarthlyBranch adjacentEarthPlate = heavenEarthPlate[l.branch];
    int count = 0;
    for (int i = 0; i < 12; ++i) {
      EarthlyBranch b = static_cast<EarthlyBranch>(
          (static_cast<int>(adjacentEarthPlate) + i) % 12);
      if (b == l.branch)
        break;
      std::vector<HeavenlyStem> heavenlyStemList =
          getHeavenlyStemsOfPalace(b);
      if (overcome(l.getFiveElements(), heavenlyStemFiveElements[l.stem])) {
        if (overcome(earthlyBranchFiveElements.at(b),
                     heavenlyStemFiveElements[l.stem])) {
          count++;
        }
        for (const auto &s : heavenlyStemList) {
          if (overcome(heavenlyStemFiveElements.at(s),
                       heavenlyStemFiveElements[l.stem])) {
            count++;
          }
        }
      } else {
        if (overcome(heavenlyStemFiveElements[l.stem],
                     earthlyBranchFiveElements.at(b))) {
          count++;
        }
        for (const auto &s : heavenlyStemList) {
          if (overcome(heavenlyStemFiveElements[l.stem],
                       heavenlyStemFiveElements.at(s))) {
            count++;
          }
        }
      }
    }
    lessonsHarmDepth.emplace_back(l, count);
  }
  int maxHarmDepth = 0;
  for (const auto &lh : lessonsHarmDepth) {
    if (lh.second > maxHarmDepth) {
      maxHarmDepth = lh.second;
    }
  }
  std::vector<StemBranch> maxHarmLessons;
  for (const auto &lh : lessonsHarmDepth) {
    if (lh.second == maxHarmDepth) {
      maxHarmLessons.push_back(lh.first);
    }
  }
  if (maxHarmLessons.size() == 1) {
    // 若只有一个涉害深度最大的课，该课的地支为初传
    initial = maxHarmLessons[0].branch;
    // 初传对应的天盘地支为中传
    middle = heavenEarthPlate[initial];
    // 中传对应的天盘地支为末传
    finalTransmission = heavenEarthPlate[middle];
    pattern.push_back(u8"涉害卦");
    return {initial, middle, finalTransmission};
  }
  // 涉害深度相同，先看是否有孟神
  for (const auto &l : maxHarmLessons) {
    EarthlyBranch adjacentEarthPlate = heavenEarthPlate[l.branch];
    if (isMeng(adjacentEarthPlate)) {
      // 有孟神，该课的地支为初传
      initial = l.branch;
      // 初传对应的天盘地支为中传
      middle = heavenEarthPlate[initial];
      // 中传对应的天盘地支为末传
      finalTransmission = heavenEarthPlate[middle];
      pattern.push_back(u8"见机卦");
      return {initial, middle, finalTransmission};
    }
  }
  // 没有孟神，看是否有仲神
  for (const auto &l : maxHarmLessons) {
    EarthlyBranch adjacentEarthPlate = heavenEarthPlate[l.branch];
    if (isZhong(adjacentEarthPlate)) {
      // 有仲神，该课的地支为初传
      initial = l.branch;
      // 初传对应的天盘地支为中传
      middle = heavenEarthPlate[initial];
      // 中传对应的天盘地支为末传
      finalTransmission = heavenEarthPlate[middle];
      pattern.push_back(u8"察微卦");
      return {initial, middle, finalTransmission};
    }
  }
  if (fourLessons.firstLesson.isYang()) {
    // 阳日，取干阳神为初传
    initial = fourLessons.stemYangGod;
    // 初传对应的天盘地支为中传
    middle = heavenEarthPlate[initial];
    // 中传对应的天盘地支为末传
    finalTransmission = heavenEarthPlate[middle];
    pattern.push_back(u8"复等卦");
    return {initial, middle, finalTransmission};
  } else {
    // 阴日，取支阳神为初传
    initial = fourLessons.branchYangGod;
    // 初传对应的天盘地支为中传
    middle = heavenEarthPlate[initial];
    // 中传对应的天盘地支为末传
    finalTransmission = heavenEarthPlate[middle];
    pattern.push_back(u8"复等卦");
    return {initial, middle, finalTransmission};
  }
  // 无法用涉害法取三传，抛出异常
  throw std::runtime_error("所临皆四季，不能用涉害取三传");
}

// 遥克法取三传
std::vector<EarthlyBranch> ThreeTransmissions::remoteOvercome() {
  if (isEightTransmissionDay(fourLessons.firstLesson)) {
    // 八传日不用遥克法，抛出异常
    throw std::runtime_error("八传日不用遥克");
  }
  std::vector<StemBranch> overcomes;
  if (overcome(heavenlyStemFiveElements[fourLessons.secondLesson.stem],
               heavenlyStemFiveElements[fourLessons.firstLesson.stem])) {
    overcomes.push_back(fourLessons.secondLesson);
  }
  if (overcome(heavenlyStemFiveElements[fourLessons.thirdLesson.stem],
               heavenlyStemFiveElements[fourLessons.firstLesson.stem])) {
    overcomes.push_back(fourLessons.thirdLesson);
  }
  if (overcome(heavenlyStemFiveElements[fourLessons.fourthLesson.stem],
               heavenlyStemFiveElements[fourLessons.firstLesson.stem])) {
    overcomes.push_back(fourLessons.fourthLesson);
  }
  if (overcomes.empty()) {
    if (overcome(heavenlyStemFiveElements[fourLessons.firstLesson.stem],
                 heavenlyStemFiveElements[fourLessons.secondLesson.stem])) {
      overcomes.push_back(fourLessons.secondLesson);
    }
    if (overcome(heavenlyStemFiveElements[fourLessons.firstLesson.stem],
                 heavenlyStemFiveElements[fourLessons.thirdLesson.stem])) {
      overcomes.push_back(fourLessons.thirdLesson);
    }
    if (overcome(heavenlyStemFiveElements[fourLessons.firstLesson.stem],
                 heavenlyStemFiveElements[fourLessons.fourthLesson.stem])) {
      overcomes.push_back(fourLessons.fourthLesson);
    }
  }
  if (overcomes.empty()) {
    // 无遥克，不能用遥克取三传，抛出异常
    throw std::runtime_error("无遥克，不能用遥克取三传");
  }
  overcomes = removeDuplicateLessons(overcomes);
  if (overcomes.size() == 1) {
    // 若只有一个遥克课，该课的地支为初传
    initial = overcomes[0].branch;
    // 初传对应的天盘地支为中传
    middle = heavenEarthPlate[initial];
    // 中传对应的天盘地支为末传
    finalTransmission = heavenEarthPlate[middle];
    pattern.push_back(u8"遥克卦");
    return {initial, middle, finalTransmission};
  } else {
    // 多个遥克课，进入比用法
    pattern.push_back(u8"遥克卦");
    return comparisonUse(overcomes);
  }
}

// 昂星法取三传
std::vector<EarthlyBranch> ThreeTransmissions::angStar() {
  std::vector<StemBranch> overcomes = {
      fourLessons.firstLesson, fourLessons.secondLesson,
      fourLessons.thirdLesson, fourLessons.fourthLesson};
  overcomes = removeDuplicateLessons(overcomes);
  if (overcomes.size() != 4) {
    // 课不备，不能用昂星取三传，抛出异常
    throw std::runtime_error("课不备，不能用昂星取三传");
  }
  if (fourLessons.firstLesson.isYang()) {
    // 阳日
    initial = heavenEarthPlate[EarthlyBranch::You];
    middle = fourLessons.branchYangGod;
    finalTransmission = fourLessons.stemYangGod;
    pattern.push_back(u8"虎视卦");
  } else {
    // 阴日
    initial = heavenEarthPlate[EarthlyBranch::You];
    middle = fourLessons.stemYangGod;
    finalTransmission = fourLessons.branchYangGod;
    pattern.push_back(u8"冬蛇掩目");
  }
  return {initial, middle, finalTransmission};
}

// 别责法取三传
std::vector<EarthlyBranch> ThreeTransmissions::specialResponsibility() {
  std::vector<StemBranch> overcomes = {
      fourLessons.firstLesson, fourLessons.secondLesson,
      fourLessons.thirdLesson, fourLessons.fourthLesson};
  overcomes = removeDuplicateLessons(overcomes);
  if (overcomes.size() == 4) {
    // 四课全备，不能用别责取三传，抛出异常
    throw std::runtime_error("四课全备，不能用别责取三传");
  }
  if (overcomes.size() != 3) {
    // 用别责用于三课备取三传，不满足条件抛出异常
    throw std::runtime_error("用别责用于三课备取三传");
  }
  if (fourLessons.firstLesson.isYang()) {
    // 阳日
    initial = heavenEarthPlate[getPalace(fourLessons.firstLesson.stem + 5)];
  } else {
    // 阴日
    initial = static_cast<EarthlyBranch>(
        (static_cast<int>(fourLessons.fourthLesson.branch) + 4) % 12);
  }
  middle = fourLessons.stemYangGod;
  finalTransmission = middle;
  pattern.push_back(u8"别责卦");
  return {initial, middle, finalTransmission};
}

// 八专法取三传
std::vector<EarthlyBranch> ThreeTransmissions::eightSpecial() {
  if (!isEightTransmissionDay(fourLessons.firstLesson)) {
    // 不是八传日，抛出异常
    throw std::runtime_error("不是八传日");
  }
  if (fourLessons.firstLesson.isYang()) {
    // 阳日
    initial = static_cast<EarthlyBranch>(
        (static_cast<int>(fourLessons.stemYangGod) + 2) % 12);
  } else {
    // 阴日
    initial = static_cast<EarthlyBranch>(
        (static_cast<int>(getBranchYinGod(fourLessons.fourthLesson.branch)) -
         2 + 12) %
        12);
  }
  middle = fourLessons.stemYangGod;
  finalTransmission = fourLessons.stemYangGod;
  pattern.push_back(u8"八专卦");
  return {initial, middle, finalTransmission};
}

// 伏吟法取三传
std::vector<EarthlyBranch> ThreeTransmissions::staticChant() {
  if (fourLessons.firstLesson.stem == HeavenlyStem::Gui) {
    // 六癸日
    initial = heavenEarthPlate[EarthlyBranch::Chou]; // 初传：以丑上神发用
    middle = EarthlyBranch::Xu;                      // 中传：丑刑戌，戌为中传
    finalTransmission = EarthlyBranch::Wei;          // 末传：戌刑未，未为末传
    if (fourLessons.firstLesson.isYang()) {
      pattern.push_back(u8"自任卦 - 伏吟 - 六癸日");
    } else {
      pattern.push_back(u8"自信卦 - 伏吟 - 六癸日");
    }
  } else if (fourLessons.firstLesson.stem == HeavenlyStem::Yi) {
    // 六乙日
    initial = EarthlyBranch::Chen;      // 初传：以辰发用
    middle = fourLessons.branchYangGod; // 中传：以日支支上神为中传

    // 查找中传的刑神作为末传
    for (int i = 1; i <= 12; ++i) {
      if (conflict(middle, static_cast<EarthlyBranch>(i - 1))) {
        finalTransmission = static_cast<EarthlyBranch>(i - 1);
        break;
      }
    }
    // 中传自刑，则以中传冲位为末传
    if (middle == finalTransmission) {
      finalTransmission =
          static_cast<EarthlyBranch>((static_cast<int>(middle) + 6) % 12);
    }
    if (fourLessons.firstLesson.isYang()) {
      pattern.push_back(u8"自任卦 - 伏吟 - 六乙日");
    } else {
      pattern.push_back(u8"自信卦 - 伏吟 - 六乙日");
    }
  } else {
    if (fourLessons.firstLesson.isYang()) {
      // 刚日
      initial = fourLessons.stemYangGod; // 初传：干上神发用

      // 查找初传的刑神作为中传
      for (int i = 1; i <= 12; ++i) {
        if (conflict(initial, static_cast<EarthlyBranch>(i - 1))) {
          middle = static_cast<EarthlyBranch>(i - 1);
          break;
        }
      }
      // 初传自刑，则以日支支上神为中传
      if (initial == middle) {
        middle = fourLessons.branchYangGod;
      }

      // 查找中传的刑神作为末传
      for (int i = 1; i <= 12; ++i) {
        if (conflict(middle, static_cast<EarthlyBranch>(i - 1))) {
          finalTransmission = static_cast<EarthlyBranch>(i - 1);
          break;
        }
      }
      // 中传自刑，则以中传冲位为末传
      if (middle == finalTransmission) {
        finalTransmission =
            static_cast<EarthlyBranch>((static_cast<int>(middle) + 6) % 12);
      }
      pattern.push_back(u8"自任卦 - 伏吟 - 刚日");
    } else {
      // 柔日
      initial = fourLessons.branchYangGod; // 初传：支上神发用

      // 查找初传的刑神作为中传
      for (int i = 1; i <= 12; ++i) {
        if (conflict(initial, static_cast<EarthlyBranch>(i - 1))) {
          middle = static_cast<EarthlyBranch>(i - 1);
          break;
        }
      }
      // 初传自刑，则以干上神为中传
      if (initial == middle) {
        middle = fourLessons.stemYangGod;
      }

      // 查找刑神作为末传
      for (int i = 1; i <= 12; ++i) {
        if (conflict(middle, static_cast<EarthlyBranch>(i - 1))) {
          finalTransmission = static_cast<EarthlyBranch>(i - 1);
          break;
        }
      }
      // 末传自刑，则以末传冲位为末传
      if (middle == finalTransmission) {
        finalTransmission =
            static_cast<EarthlyBranch>((static_cast<int>(middle) + 6) % 12);
      }
      pattern.push_back(u8"自信卦 - 伏吟 - 柔日");
    }
  }
  return {initial, middle, finalTransmission};
}

// 返吟法取三传
std::vector<EarthlyBranch> ThreeTransmissions::reverseChant() {
  try {
    // 先尝试用贼克法取三传
    return thiefConquer();
  } catch (const std::exception &) {
    // 驿马计算
    EarthlyBranch branch = fourLessons.fourthLesson.branch;
    for (int i = 0; i < 3; ++i) {
      if (isYinShenSiHai(branch)) {
        initial =
            static_cast<EarthlyBranch>((static_cast<int>(branch) + 6) % 12);
        break;
      }
      branch =
          static_cast<EarthlyBranch>((static_cast<int>(branch) + 4) % 12);
    }
    middle = fourLessons.branchYangGod;
    finalTransmission = fourLessons.stemYangGod;
    pattern.push_back(u8"无依卦");
    return {initial, middle, finalTransmission};
  }
}

// 判断地支是否为孟神（寅、巳、申、亥）
bool ThreeTransmissions::isMeng(EarthlyBranch branch) const {
  return branch == EarthlyBranch::Yin || branch == EarthlyBranch::Si ||
         branch == EarthlyBranch::Shen || branch == EarthlyBranch::Hai;
}

// 判断地支是否为仲神（子、卯、午、酉）
bool ThreeTransmissions::isZhong(EarthlyBranch branch) const {
  return branch == EarthlyBranch::Zi || branch == EarthlyBranch::Mao ||
         branch == EarthlyBranch::Wu || branch == EarthlyBranch::You;
}

// 判断干支是否为八传日
bool ThreeTransmissions::isEightTransmissionDay(const StemBranch &stemBranch) const {
  // 八专日判断逻辑，假设某些特定干支为八专日
  static const std::vector<StemBranch> eightTransmissionDayList = {
      StemBranch(HeavenlyStem::Jia, EarthlyBranch::Zi),
      StemBranch(HeavenlyStem::Yi, EarthlyBranch::Chou),
      // 补充其他八专日干支
  };
  return std::find(eightTransmissionDayList.begin(),
                   eightTransmissionDayList.end(),
                   stemBranch) != eightTransmissionDayList.end();
}

// 判断是否为伏吟课
bool ThreeTransmissions::isStaticChantLesson() const {
  for (int i = 0; i < 12; ++i) {
    EarthlyBranch currentBranch = static_cast<EarthlyBranch>(i);
    if (heavenEarthPlate[currentBranch] != currentBranch) {
      return false;
    }
  }
  return true;
}

// 判断是否为返吟课
bool ThreeTransmissions::isReverseChantLesson() const {
  for (int i = 0; i < 12; ++i) {
    EarthlyBranch currentBranch = static_cast<EarthlyBranch>(i);
    EarthlyBranch oppositeBranch = static_cast<EarthlyBranch>((i + 6) % 12);
    if (heavenEarthPlate[currentBranch] != oppositeBranch) {
      return false;
    }
  }
  return true;
}

// 三传类构造函数，根据四课和天地盘信息计算三传
ThreeTransmissions::ThreeTransmissions(const HeavenEarthPlate &he, const FourLessons &s)
    : heavenEarthPlate(he), fourLessons(s) {
  std::vector<EarthlyBranch> result;

  // 优先处理伏吟课
  if (isStaticChantLesson()) {
    result = staticChant();
  }
  // 其次处理返吟课
  else if (isReverseChantLesson()) {
    result = reverseChant();
  }
  // 正常课式处理流程
  else {
    try {
      // 1. 先尝试贼克法（含比用、涉害）
      result = thiefConquer();
    } catch (const std::exception &e) {
      try {
        // 2. 尝试遥克法
        result = remoteOvercome();
      } catch (const std::exception &e) {
        try {
          // 3. 尝试昴星法
          result = angStar();
        } catch (const std::exception &e) {
          try {
            // 4. 尝试别责法
            result = specialResponsibility();
          } catch (const std::exception &e) {
            try {
              // 5. 处理八专日
              result = eightSpecial();
            } catch (const std::exception &e) {
              throw std::runtime_error("所有方法均无法确定三传");
            }
          }
        }
      }
    }
  }

  // 赋值三传
  if (result.size() == 3) {
    initial = result[0];
    middle = result[1];
    finalTransmission = result[2];
  } else {
    throw std::runtime_error("三传结果异常");
  }
}

// 获取初传地支
EarthlyBranch ThreeTransmissions::getInitial() const { return initial; }
// 获取中传地支
EarthlyBranch ThreeTransmissions::getMiddle() const { return middle; }
// 获取末传地支
EarthlyBranch ThreeTransmissions::getFinalTransmission() const { return finalTransmission; }
// 获取三传的格局类型
const std::vector<std::u8string> &ThreeTransmissions::getPattern() const { return pattern; }