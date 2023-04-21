#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

using namespace std;
using std::string;
clock_t ClockStart, ClockEnd;
ofstream file;

mutex mtx;

/*void writeFile(//提升可读性的换行
    const char* contentChar1,int contentChar2,
    const char* contentChar3,int contentChar4,
    const char* contentChar5,int contentChar6,
    const char* contentChar7,int contentChar8,
    const char* contentChar9,int contentChar10,
    const char* contentChar11,int contentChar12,
    const char* File
    ) {
    ofstream file;
    file.open(File, ios::out | ios::app);
    file << contentChar1 << contentChar2 << contentChar3 << contentChar4 << contentChar5 << contentChar6 << contentChar7 << contentChar8 << contentChar9 << contentChar10 << contentChar11 << contentChar12 << endl;
    file.close();
    return;
}*/

bool isNumber(const string& str) //判断你输入的字符串是否由纯数字组成
{
    for (char const& c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

bool isZero(std::string stringNum) {
    std::string cmpStr = "0";//初始化
    std::string ThisStr = "0";//初始化
    int count = 0;
    for (int i = 0; i <= stringNum.length(); i++) {//遍历字符
        cmpStr = stringNum.substr(i, 1);//截取字符
        if (cmpStr != "0" && !count) {
            count = 1;//当检测到第一个有效数字时更改这个变量来进行字符串存储
        }
        if (count && cmpStr != "0") {
            ThisStr = ThisStr + cmpStr;
        }
    }
    if (ThisStr.length() < 11) return true; else return false;//大小判断
}
int getNumber(const char* speaky) { //自定义一个询问并获取数字的函数
    std::string stringNum = "0";
    char charNum[127] = { '\0' };
    while (true)
    {
        printf(speaky);
        scanf_s("%s", charNum);
        std::string stringNum = charNum;
        if (stringNum.length() < 10 && isNumber(charNum) || isZero(stringNum)) //数字长度判断和零的判断，避免异常
        {
            if (isNumber(stringNum)) {
                return std::stoi(stringNum);//获取正确的整数
            }
            printf("参数不为纯数字，请重新输入！\n");
        }
        else if (!isNumber(charNum)) {//字符串过长不显示数字过大
            printf("参数不为纯数字，请重新输入！\n");
        }
        else {
            printf("数字过大(>999999999)，请重新输入！\n");
        }
    }
}
const char* getChar(const char* speaky) { //自定义一个询问并获取字符串的函数
    char charNum[127] = { '\0' };
    printf(speaky);
    scanf_s("%s", charNum);
    return charNum;
}

int getModMax(const char* speaky, int modMax, int outInt = 0) { //自定义一个询问并返回特定数字的函数
    char charNum[2] = { '\0' };
    std::string stringNum = "0";
    while (true)
    {
        printf(speaky);
        scanf_s("%s", charNum);//先用scanf_s获得值
        std::string stringNum = charNum;//然后赋值给string
        if (stringNum == "y") {
            return modMax;
        }
        else if (stringNum == "n") {
            return outInt;
        }
        else {
            printf("你需要输入的是y或n，而不是其他参数！\n");
        }
    }
}


class cfgClass {//配置文件读取
public:
    cfgClass() {
        cfg.open(cfgFile, ios::in);
        if (!cfg.is_open())//如果没有文件就新建一个！
        {
            cfg.close();
            cfg.open(cfgFile, ios::out | ios::app);
            cfg << "threadNum{1}" << endl;
            cfg << "//更改线程数量，一线程为主线程，二线程为主线程+子线程，如此类推下去" << endl;
            cfg << "//警告：本功能可能不稳定会导致崩溃，而且有可能性能提升不明显，如果为1那么就只有主线程运算" << endl;
            cfg << "//所以，拿来玩玩倒是可以就对了（）" << endl;
            printf("已检测到你没有配置文件，已经自动生成了一个默认的配置文件\n");
            cfg.close();
            cfg.open(cfgFile, ios::in);
        }
        while (cfg.getline(buf, sizeof(buf)))//按行读文件 这里存入参数的原理为，将参数线性存入向量cfgStr和向量cfgNumInt1/2中，然后循环到符合其中一个时，这时候累加器的参数写入向量下标进行寻址时就等同于该参数的其他数据了
        {
            if (!(buf[0] == '/' && buf[1] == '/') && !buf[0] == '\0') { //判断注释
                if (!(strchr(buf, '{') && strchr(buf, '}')) || (strchr(buf, '}') - buf) - (strchr(buf, '{') - buf) == 1) {//如果配置文件语法不正确报错，只报一次，最右边的条件判断语句是判断是否只是单纯的{}组合
                    if (decideSyntax){
                        cerr << "检查到不符合配置文件语法的内容，已跳过" << endl;
                        decideSyntax = false;
                    }
                    continue;//退出这次循环，避免异常
                }
                duplicateStr = false;
                //cout << buf << endl;//这些cout是调试用的，所以被注释掉了
                count++;
                memset(buf2, '\0', sizeof(buf2));//清空缓存
                charPos[0] = strchr(buf, '{') - buf;//计算位置
                charPos[1] = strchr(buf, '}') - buf;
                //cout << charPos[0] << endl;
                //cout << charPos[1] << endl;
                for (int i = 0; i < charPos[0]; i++) {//存入参数名
                    buf2[i] = buf[i];
                }
                if (!cfgStr.empty()) {
                    bufStr = buf2;//判断参数名是否重复
                    for (int i = 0; i < cfgStr.size();i++) {
                        if (bufStr == cfgStr[i]) {
                            duplicateStr = true;
                            bufInt = i;
                            cfgNumInt2[bufInt]++;
                            count--;//恢复正常计数
                        }
                    }
                }
                if (!duplicateStr) {
                    cfgStr.push_back(buf2);//如果不为重复，则把参数名存入向量中
                }
                memset(buf2, '\0', sizeof(buf2));//重置
                for (int i = charPos[0]+1,i2 = 0; i < charPos[1]; i++,i2++) {
                    buf2[i2] = buf[i];//取出参数
                }
                bufStr = buf2;
                if (!duplicateStr){//存入参数
                    if (bufStr.length() < 4 && isNumber(buf2) && isZero(bufStr)) //数字长度判断和零的判断，避免异常
                    {
                        if (isNumber(bufStr)) {
                            cfgNumInt1.push_back(stoi(bufStr));
                            cfgNumInt2.push_back(0);//重复累加器，默认是0，先要压入
                        }
                        else{
                            cfgNumInt1.push_back(1);
                            cerr << "警告:参数" << cfgStr[count] << "输入的参数不为纯数字，已更换成1" << endl;
                            cfgNumInt2.push_back(0);
                        }
                    }
                    else if (!isNumber(buf2)) {//字符串过长不显示数字过大
                        cfgNumInt1.push_back(1);
                        cerr << "警告:参数" << cfgStr[count] << "输入的参数不为纯数字，已更换成1" << endl;
                        cfgNumInt2.push_back(0);
                    }
                    else {
                        cfgNumInt1.push_back(1);
                        cerr << "警告:参数" << cfgStr[count] << "输入的参数过大(>999)，已更换成1" << endl;
                        cfgNumInt2.push_back(0);
                    }
                }
                else {//如果是重复定义的就重新赋值
                    if (bufStr.length() < 4 && isNumber(buf2) && isZero(bufStr)) //数字长度判断和零的判断，避免异常
                    {
                        if (isNumber(bufStr)) {
                            cfgNumInt1[bufInt] = stoi(bufStr);
                        }
                        else {
                            cfgNumInt1[bufInt] = 1;
                            cerr << "警告:参数" << cfgStr[bufInt] << "输入的参数不为纯数字，已更换成1" << endl;
                        }
                    }
                    else if (!isNumber(buf2)) {//字符串过长不显示数字过大
                        cfgNumInt1[bufInt] = 1;
                        cerr << "警告:参数" << cfgStr[bufInt] << "输入的参数不为纯数字，已更换成1" << endl;
                    }
                    else {
                        cfgNumInt1[bufInt] = 1;
                        cerr << "警告:参数" << cfgStr[bufInt] << "输入的参数过大(>999)，已更换成1" << endl;
                    }
                }
                //cout << cfgNumInt1[count] << endl;
                //cout << cfgNumInt2[count] << endl;
            }
        }
        cfg.close();//关闭文件
        for (int i = 0; i < cfgNumInt2.size(); i++) {
            if (cfgNumInt2[i] != 0) {
                cerr << "警告:参数" << cfgStr[i] << "在配置文件中被重复定义了" << cfgNumInt2[i] << "次，" << "程序将选择最后被定义的数字:" << cfgNumInt1[i] << endl;
            }
        }
    }
    int getParameter(string Par) {//输入字符串，获取参数
        if (!cfgStr.empty()) {
            for (int i = 0; i < cfgStr.size(); i++) {
                if (Par == cfgStr[i]) {
                    return cfgNumInt1[i];
                }
            }
        }
        if (cfgStr.empty()) {
            cerr << "配置文件里为什么没有参数呢:("<< "\n参数:" << Par << "将返回默认值1\n建议把配置文件删了然后重新打开本程序\n这样的话会新建一个默认的配置文件" << endl;
            return 1;
        }
        cerr << "未找到参数:" << Par << "，将返回默认值1" << endl;
        return 1;
    }
private:
    fstream cfg;
    string cfgFile = "infwisp.cfg";
    int charPos[2];
    char buf[256] = { '\0' };//缓存
    char buf2[256] = { '\0' };
    string bufStr = "0";
    int count = -1;
    bool duplicateStr = false;//判重用的变量
    bool decideSyntax = true;
    int bufInt;
    vector<string> cfgStr;//定义一个用于存储参数的字符串向量
    vector<int> cfgNumInt1;
    vector<int> cfgNumInt2;//累加重复次数的
};

atomic<int> HasAnw = 0;

void threadCalc(int mod,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT) {
    int out = 1;
    int YouNeedNum;
    int Count = 0;
    int test;
    int add2 = 0;
    int pin2 = 0;
    int hel2 = 0;
    int arc2 = 0;
    if (pinLT != 0 && addLT == 0) {
        pin2 = mod;
    }
    else if (helLT != 0 && addLT == 0 && pinLT == 0) {
        hel2 = mod;
    }
    else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
        arc2 = mod;
    }
    else {
        add2 = mod;
    }
    if (splLT != 0 && decLT != 0) {
        int PreNumI = -decLT * 42 - splLT * 30 + endNum;//预计算，减少性能消耗
        for (int add = add2; add <= addLT; add++)
        {
            for (int pin = pin2; pin <= pinLT; pin++)
            {
                for (int hel = hel2; hel <= helLT; hel++)
                {
                    for (int arc = arc2; arc <= arcLT; arc++)
                    {
                        for (int dec = 0; dec <= decLT; dec++)
                        {
                            if (Count > 1) { out = 1; }//重置out，避免优化算法永远不起作用
                            Count = 0;
                            for (int spl = 0; spl <= splLT; spl++)
                            {
                                YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42 - spl * 30);
                                if (YouNeedNum < 0 && out) {
                                    test = startNum / 30;
                                    if (test == 1) { out = 0; }
                                    if (startNum % 30 == 0) { out = 0; }//如果不加这两个判断在特定条件下会陷入死循环
                                    if (Count > 1) { out = 0; }//卡死判断，即此处连续执行超过两次就退出
                                    spl = test - 1;
                                    Count++;
                                    continue;
                                }
                                if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                    HasAnw++;//如果有了可以永久化的结果自增
                                    if (isSaveOrNo) {
                                        mtx.lock();
                                        file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << spl << endl;
                                        mtx.unlock();
                                    }
                                    else {
                                        mtx.lock();
                                        printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n", add, pin, hel, arc, dec, spl); 
                                        mtx.unlock();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (splLT == 0 && decLT != 0) {//方案2 只计算减去时间修正中的红表，计算红表的最小值，然后直接赋值跳过无用循环
        int PreNumI = -decLT * 42 + endNum;//预计算，减少性能消耗
        for (int add = add2; add <= addLT; add++)
        {
            for (int pin = pin2; pin <= pinLT; pin++)
            {
                for (int hel = hel2; hel <= helLT; hel++)
                {
                    for (int arc = arc2; arc <= arcLT; arc++)
                    {
                        if (Count > 1) { out = 1; }
                        Count = 0;
                        for (int dec = 0; dec <= decLT; dec++)
                        {
                            YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42);
                            if (YouNeedNum < 0 && out) {
                                test = startNum / 42;
                                if (test == 1) { out = 0; }
                                if (startNum % 42 == 0) { out = 0; }
                                if (Count > 1) { out = 0; }
                                dec = test - 1;
                                Count++;
                                continue;
                            }
                            if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                HasAnw++;//如果有了可以永久化的结果自增
                                if (isSaveOrNo) {
                                    mtx.lock();
                                    file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << splLT << endl;
                                    mtx.unlock();
                                }
                                else {
                                    mtx.lock();
                                    printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n", add, pin, hel, arc, dec, splLT);
                                    mtx.unlock();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (splLT != 0 && decLT == 0) {//方案3 只计算减去时间修正中的连锁，计算连锁的最小值，然后直接赋值跳过无用循环
        int PreNumI = -decLT * 30 + endNum;//预计算，减少性能消耗
        for (int add = add2; add <= addLT; add++)
        {
            for (int pin = pin2; pin <= pinLT; pin++)
            {
                for (int hel = hel2; hel <= helLT; hel++)
                {
                    for (int arc = arc2; arc <= arcLT; arc++)
                    {
                        if (Count > 1) { out = 1; }
                        Count = 0;
                        for (int spl = 0; spl <= splLT; spl++)
                        {
                            YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - spl * 30);
                            if (YouNeedNum < 0 && out) {
                                test = startNum / 30;
                                if (test == 1) { out = 0; }
                                if (startNum % 30 == 0) { out = 0; }
                                if (Count > 1) { out = 0; }
                                spl = test - 1;
                                Count++;
                                continue;
                            }
                            if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                HasAnw++;//如果有了可以永久化的结果自增
                                if (isSaveOrNo) {
                                    mtx.lock();
                                    file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << decLT << ",连锁法术数量:" << spl << endl;
                                    mtx.unlock();
                                }
                                else {
                                    mtx.lock();
                                    printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n", add, pin, hel, arc, decLT, spl);
                                    mtx.unlock();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}
cfgClass cfg;//定义为全局的
vector<thread> T;//动态存储线程
vector<int> TNum;//动态存储分配后的参数

int main() {//addLT蓝表,pinLT乒乓,decLT红表，helLT螺旋魔弹,arcLT相位弧度,splLT连锁法术
    SetConsoleTitle(L"永久法术计算工具v1.0.7");//修改控制台标题
    printf("永久法术计算工具v1.0.7\n\n注:乒乓回弹和盘旋魔弹影响的存在时间数值一样\n本程序的Github仓库链接:https://github.com/KagiamamaHIna/noita-infiniwisp-calculator 可以前来下最新版本或者查看源代码\n本程序使用MIT许可证\n\n");
    int startNum, endNum, modMax, addLT, pinLT, decLT, helLT, arcLT, splLT, YouNeedNum, isSaveOrNo, isFileCustOrNo = 0;
    int closeNum = 1, test = 1;
    const char* File = "infwispList.txt";
    int Count = 0;
    int threadNum = cfg.getParameter("threadNum");//为了减少性能消耗，获取一次赋值给一个变量
    //int cacheWrite = cfg.getParameter("cacheWrite");
    while (true)
    {
        int threadNum2[2] = { 0,0 };//第一个存总数，第二个存余数
        int out = 1;
        HasAnw = 0;
        int isYes = 0;
        int YouShouldNum;
        startNum = getNumber("输入投射物存在时间范围的起始值:");
        while (true) {
            endNum = getNumber("输入投射物存在时间范围的终止值:");
            if (startNum <= endNum) { //判断终止值不能小于起始值
                break;
            }
            printf("终止值不能小于起始值。\n");
        }
        modMax = getNumber("输入所有影响存在时间修正的上限数:");
        isSaveOrNo = getModMax("如果不需要将数值存储到文件中输入n,需要则输入y:", 1, 0);
        if (isSaveOrNo) {
            if (closeNum == 0 && isFileCustOrNo == 1) { //判断是否自定义过路径并且已经运行过一遍的代码
                isFileCustOrNo = getModMax("如果不需要更改文件路径输入n,需要则输入y:", 1, 0);
            }
            else {
                isFileCustOrNo = getModMax("如果不需要自定义文件路径输入n,需要则输入y:", 1, 0);
            }
            if (isFileCustOrNo) {
                File = getChar("请输入文件路径:");
            }
            printf("tips:接下来将不会在控制台内打印数据，而是将数据保存入根路径/%s 文件中\n", File);
        }
        addLT = getModMax("如果不需要计算蓝表输入n,需要计算输入y:", modMax);//根据你所输入的改变可循环的最大值，以此实现无需计算
        pinLT = getModMax("如果不需要计算乒乓输入n,需要计算输入y:", modMax);
        decLT = getModMax("如果不需要计算红表输入n,需要计算输入y:", modMax);
        helLT = getModMax("如果不需要计算螺旋魔弹输入n,需要计算输入y:", modMax);
        arcLT = getModMax("如果不需要计算相位输入n,需要计算输入y:", modMax);
        splLT = getModMax("如果不需要计算连锁输入n,需要计算输入y:", modMax);
        if (isSaveOrNo) {
            file.open(File, ios::out | ios::app);
            file << "本次输入的存在时间范围为:" << startNum << "到" << endNum << ",总修正上限为" << modMax << endl;
            file << "蓝表上限为:" << addLT << endl;
            file << "乒乓上限为:" << pinLT << endl;
            file << "螺旋魔弹上限为:" << helLT << endl;
            file << "相位上限为:" << arcLT << endl;
            file << "红表上限为:" << decLT << endl;
            file << "连锁上限为:" << splLT << endl;
        }
        //穷举计算 一共四种方案
        //方案1 两种减去时间的修正都计算，计算连锁的最小值，然后直接赋值跳过无用循环
        if (splLT != 0 && decLT != 0) {
            YouShouldNum = startNum / 42;
            if (decLT < YouShouldNum) {
                isYes = 1;
                goto calcEnd;
            }
            int LT;
            int PreNumI = -decLT * 42 - splLT * 30 + endNum;//预计算，减少性能消耗
            if (pinLT != 0 && addLT == 0) {
                LT = ceil(-((float(PreNumI) - 25) / 25));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;//计算线程数和需要分配的参数
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                pinLT = TNum[0];
            }
            else if (helLT != 0 && addLT == 0 && pinLT==0) {
                LT = ceil(-((float(PreNumI) - 50) / 50));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                helLT = TNum[0];
            }
            else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                LT = ceil(-((float(PreNumI) - 80) / 80));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                arcLT = TNum[0];
            }
            else {
                LT = ceil(-((float(PreNumI) - 75) / 75));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                addLT = TNum[0];
            }
            if (threadNum > 1) {
                T = vector<thread>(threadNum - 1); //初始化线程数量，主线程也算一个，所以-1
                int count2[2] = { TNum[0],0 };
                for (int i = 0; i < T.size(); i++) {//9 9 8 8(34)
                    count2[1] = count2[0]+1;
                    count2[0] = count2[0] + TNum[i + 1];//算应该分配的范围
                    if (pinLT != 0 && addLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, count2[0], decLT, helLT, arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, count2[0], arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, count2[0], splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, count2[0], pinLT, decLT, helLT, arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    //cout << count2[1] << "/0:" << count2[0] << endl;//debug用
                }
            }
            ClockStart = clock();
            for (int add = 0; add <= addLT; add++)
            {/*
                if (add * 75 + PreNumI >= 75) {//如果条件满足，即增加时间修正过大的话，就直接跳过下列所有结果直接输出，因为往下计算均是无效结果，>=75是为了减少一次减法运算带来的性能消耗
                    goto calcEnd;
                }*/
                for (int pin = 0; pin <= pinLT; pin++)
                {/*
                    if (addLT == 0 && pin * 25 + PreNumI >= 25) {
                        goto calcEnd;
                    }*/
                    for (int hel = 0; hel <= helLT; hel++)
                    {/*
                        if (addLT == 0 && pinLT == 0 && hel * 50 + PreNumI >= 50) {
                            goto calcEnd; 
                        }*/
                        for (int arc = 0; arc <= arcLT; arc++)
                        {/*
                            if (addLT == 0 && pinLT == 0 && helLT == 0 && arc * 80 + PreNumI >= 80) {
                                goto calcEnd; 
                            }*/
                            for (int dec = 0; dec <= decLT; dec++)
                            {
                                if (Count > 1) { out = 1; }//重置out，避免优化算法永远不起作用
                                Count = 0;
                                for (int spl = 0; spl <= splLT; spl++)
                                {
                                    YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42 - spl * 30);
                                    if (YouNeedNum < 0 && out) {
                                        test = startNum / 30;
                                        if (test == 1) { out = 0; }
                                        if (startNum % 30 == 0) { out = 0; }//如果不加这两个判断在特定条件下会陷入死循环
                                        if (Count > 1) { out = 0; }//卡死判断，即此处连续执行超过两次就退出
                                        spl = test - 1;
                                        Count++;
                                        continue;
                                    }
                                    if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                        HasAnw++;//如果有了可以永久化的结果自增
                                        if (isSaveOrNo) {
                                            mtx.lock();
                                            file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << spl << endl;
                                            mtx.unlock();
                                        }
                                        else {
                                            mtx.lock();
                                            printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n", add, pin, hel, arc, dec, spl);
                                            mtx.unlock();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (splLT == 0 && decLT != 0) {//方案2 只计算减去时间修正中的红表，计算红表的最小值，然后直接赋值跳过无用循环
            YouShouldNum = startNum / 42;
            if (decLT < YouShouldNum) {
                isYes = 1;
                goto calcEnd;
            }
            int PreNumI = -decLT * 42 + endNum;//预计算，减少性能消耗
            int LT;
            if (pinLT != 0 && addLT == 0) {
                LT = ceil(-((float(PreNumI) - 25) / 25));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;//计算线程数和需要分配的参数
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                pinLT = TNum[0];
            }
            else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                LT = ceil(-((float(PreNumI) - 50) / 50));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                helLT = TNum[0];
            }
            else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                LT = ceil(-((float(PreNumI) - 80) / 80));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                arcLT = TNum[0];
            }
            else {
                LT = ceil(-((float(PreNumI) - 75) / 75));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                addLT = TNum[0];
            }
            if (threadNum > 1) {
                T = vector<thread>(threadNum - 1); //初始化线程数量，主线程也算一个，所以-1
                int count2[2] = { TNum[0],0 };
                for (int i = 0; i < T.size(); i++) {//9 9 8 8(34)
                    count2[1] = count2[0]+1;
                    count2[0] = count2[0] + TNum[i + 1];//算应该分配的范围
                    if (pinLT != 0 && addLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, count2[0], decLT, helLT, arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, count2[0], arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, count2[0], splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, count2[0], pinLT, decLT, helLT, arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                }
            }
            ClockStart = clock();
            for (int add = 0; add <= addLT; add++)
            {
                for (int pin = 0; pin <= pinLT; pin++)
                {
                    for (int hel = 0; hel <= helLT; hel++)
                    {
                        for (int arc = 0; arc <= arcLT; arc++)
                        {
                            if (Count > 1) { out = 1; }
                            Count = 0;
                            for (int dec = 0; dec <= decLT; dec++)
                            {
                                YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42);
                                if (YouNeedNum < 0 && out) {
                                    test = startNum / 42;
                                    if (test == 1) { out = 0; }
                                    if (startNum % 42 == 0) { out = 0; }
                                    if (Count > 1) { out = 0; }
                                    dec = test - 1;
                                    Count++;
                                    continue;
                                }
                                if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                    HasAnw++;//如果有了可以永久化的结果自增
                                    if (isSaveOrNo) {
                                        mtx.lock();
                                        file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << splLT << endl;
                                        mtx.unlock();
                                    }
                                    else {
                                        mtx.lock();
                                        printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n", add, pin, hel, arc, dec, splLT);
                                        mtx.unlock();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (splLT != 0 && decLT == 0) {//方案3 只计算减去时间修正中的连锁，计算连锁的最小值，然后直接赋值跳过无用循环
            YouShouldNum = startNum / 30;
            if (splLT < YouShouldNum) {
                isYes = 1;
                goto calcEnd;
            }
            int PreNumI = -decLT * 30 + endNum;//预计算，减少性能消耗
            int LT;
            if (pinLT != 0 && addLT == 0) {
                LT = ceil(-((float(PreNumI) - 25) / 25));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;//计算线程数和需要分配的参数
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                pinLT = TNum[0];
            }
            else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                LT = ceil(-((float(PreNumI) - 50) / 50));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                helLT = TNum[0];
            }
            else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                LT = ceil(-((float(PreNumI) - 80) / 80));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                arcLT = TNum[0];
            }
            else {
                LT = ceil(-((float(PreNumI) - 75) / 75));//获取上限修正
                TNum = vector<int>(threadNum);
                threadNum2[0] = LT / threadNum;
                threadNum2[1] = LT % threadNum;
                for (int i = 0; i < threadNum; i++) {
                    TNum[i] = threadNum2[0];
                }
                if (threadNum2[1] > 0) {
                    for (int i = 0; i < threadNum2[1]; i++) {
                        TNum[i]++;
                    }
                }
                addLT = TNum[0];
            }
            if (threadNum > 1) {
                T = vector<thread>(threadNum - 1); //初始化线程数量，主线程也算一个，所以-1
                int count2[2] = { TNum[0],0 };
                for (int i = 0; i < T.size(); i++) {//9 9 8 8(34)
                    count2[1] = count2[0]+1;
                    count2[0] = count2[0] + TNum[i + 1];//算应该分配的范围
                    if (pinLT != 0 && addLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, count2[0], decLT, helLT, arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, count2[0], arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, count2[0], splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                    else {
                        T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, count2[0], pinLT, decLT, helLT, arcLT, splLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                    }
                }
            }
            ClockStart = clock();
            for (int add = 0; add <= addLT; add++)
            {
                for (int pin = 0; pin <= pinLT; pin++)
                {
                    for (int hel = 0; hel <= helLT; hel++)
                    {
                        for (int arc = 0; arc <= arcLT; arc++)
                        {
                            if (Count > 1) { out = 1; }
                            Count = 0;
                            for (int spl = 0; spl <= splLT; spl++)
                            {
                                YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - spl * 30);
                                if (YouNeedNum < 0 && out) {
                                    test = startNum / 30;
                                    if (test == 1) { out = 0; }
                                    if (startNum % 30 == 0) { out = 0; }
                                    if (Count > 1) { out = 0; }
                                    spl = test - 1;
                                    Count++;
                                    continue;
                                }
                                if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                    HasAnw++;//如果有了可以永久化的结果自增
                                    if (isSaveOrNo) {
                                        mtx.lock();
                                        file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << decLT << ",连锁法术数量:" << spl << endl;
                                        mtx.unlock();
                                    }
                                    else {
                                        mtx.lock();
                                        printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n", add, pin, hel, arc, decLT, spl);
                                        mtx.unlock();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }//如果条件均不满足，那么就不循环了，直接结束，也就是没有减存在时间的法术的时候
        calcEnd:ClockEnd = clock();
        if (threadNum > 1 && !isYes){
             for (int i = 0; i < T.size(); i++) {
                  T[i].join();//释放线程
             }
        }
        float time = float(ClockEnd - ClockStart) / 1000;//我将原本的换成了1000作为常量，因为我听说其他情况机子跑编译的情况下可能不为1000
        if (isYes) {
            printf("共耗时：0.00000s，你输入的修正数过少，建议在等条件的情况下输入%d\n", YouShouldNum);
        }
        else{
        printf("共耗时：%.5fs\n", time);
        }
        int HasAnw2 = HasAnw;
        if (HasAnw2 == 0) {//有了可以永久化的结果就按条件输出语句
            printf("这次穷举没有可以永久化的结果，你输入的存在时间范围为: %d 到 %d \n\n", startNum, endNum);
            if (isSaveOrNo) {
                file << "本次穷举没有可以永久化的结果:(" << endl;
                file.close();
            }
        }
        else {
            printf("这次穷举有可以永久化的结果，结果数量为:%d，你输入的存在时间范围为: %d 到 %d \n\n", HasAnw2, startNum, endNum);
            if (isSaveOrNo) {
                file << "共计" << HasAnw2 << "个结果" << endl;
                file.close();
            }
        }
        closeNum = getModMax("如果要继续计算请输入y, 退出输入n:", 0, 1);
        if (closeNum) {
            break;
        }
    }
    return 0;
}
