#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;
using std::string;
clock_t ClockStart, ClockEnd;
ofstream file;

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
int getNumber(const char *speaky) { //自定义一个询问并获取数字的函数
    std::string stringNum = "0";
    char charNum[127] = { '\0' };
    while (true)
    {
        printf(speaky);
        scanf_s("%s", charNum);
        std::string stringNum = charNum;
        if (stringNum.length() < 10) //数字长度判断，避免异常
        {
            if (isNumber(stringNum)) {
                return std::stoi(stringNum);//获取正确的整数
            }
            printf("参数不为纯数字，请重新输入！\n");
        }
        else{
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

int getModMax(const char* speaky,int modMax,int outInt = 0) { //自定义一个询问并返回特定数字的函数
    char charNum[2] = { '\0' };
    std::string stringNum = "0";
    std::string y = "y";//定义两个用于比较的变量
    std::string n = "n";
    while (true)
    {
        printf(speaky);
        scanf_s("%s", charNum);//先用scanf_s获得值
        std::string stringNum = charNum;//然后赋值给string
        if (stringNum == y) {
            return modMax;
        }
        else if (stringNum == n) {
            return outInt;
        }
        else {
            printf("你需要输入的是y或n，而不是其他参数！\n");
        }
    }
}

int main() {//addLT蓝表,pinLT乒乓,decLT红表，helLT螺旋魔弹,arcLT相位弧度,splLT连锁法术
    SetConsoleTitle(L"永久法术计算工具v1.0.4");
    printf("永久法术计算工具v1.0.4\n\n注:乒乓回弹和盘旋魔弹影响的存在时间数值一样\n本程序的Github仓库链接:https://github.com/KagiamamaHIna/noita-infiniwisp-calculator 可以前来下最新版本或者查看源代码\n本程序使用MIT许可证\n\n");
    int startNum,endNum,modMax,addLT,pinLT,decLT,helLT,arcLT,splLT,YouNeedNum,isSaveOrNo,isFileCustOrNo = 0;
    int closeNum = 1;
    const char* File = "infwispList.txt";
    while (true)
    {
        int HasAnw = 0;
        startNum = getNumber("输入投射物存在时间范围的起始值:");
        while (true){
            endNum = getNumber("输入投射物存在时间范围的终止值:");
            if (startNum <= endNum) { //判断终止值不能小于起始值
                break;
            }
            printf("终止值不能小于起始值。\n");
        }
        modMax = getNumber("输入所有影响存在时间修正的上限数:");
        isSaveOrNo = getModMax("如果不需要将数值存储到文件中输入n,需要则输入y:",1,0);
        if (isSaveOrNo) {
            if (closeNum == 0 && isFileCustOrNo == 1){ //判断是否自定义过路径并且已经运行过一遍的代码
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
        file << "本次输入的存在时间范围为:" << startNum << "到" << endNum << ",总修正上限为"<< modMax << endl;
        file << "蓝表上限为:" << addLT << endl;
        file << "乒乓上限为:" << pinLT << endl;
        file << "螺旋魔弹上限为:" << helLT << endl;
        file << "相位上限为:" << arcLT << endl;
        file << "红表上限为:" << decLT << endl;
        file << "连锁上限为:" << splLT << endl;
        }
        unsigned long int cycleAll = 1;
        unsigned long int LT[6] = { addLT ,pinLT , helLT , arcLT , decLT , splLT };
        int count = 0;
        for (int i = 0; i <= 6; i++) {
            if (LT[i] != 0){ //排除0的选项，计算总共的循环次数
                cycleAll *= LT[i];
            }
            else{
                count++;//累加器
            }
            if (count == 6) {//如果全是0，那么代表没有循环
                cycleAll = 0;
            }
        }
        //穷举计算
        ClockStart = clock();
        for (int add = 0; add <= addLT; add++)
        {
            for (int pin = 0; pin <= pinLT; pin++)
            {
                for (int hel = 0; hel <= helLT; hel++)
                {
                    for (int arc = 0; arc <= arcLT; arc++)
                     {
                        for (int dec = 0; dec <= decLT; dec++)
                        {
                            for (int spl = 0; spl <= splLT; spl++)
                            {
                                YouNeedNum = -(75*add+25*pin+50*hel+80*arc-dec*42-spl*30);
                                if (YouNeedNum >= startNum+1 && YouNeedNum <= endNum+1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                    HasAnw++;//如果有了可以永久化的结果自增
                                    if (isSaveOrNo) {
                                        file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << spl << endl;
                                    }
                                    else{
                                    printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n",add,pin,hel,arc,dec,spl);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        ClockEnd = clock();
        double time = double(ClockEnd - ClockStart) / CLOCKS_PER_SEC;
        printf("共循环:%llu，共耗时：%.5fs\n",cycleAll, time);
        if (HasAnw == 0) {//有了可以永久化的结果就按条件输出语句
            printf("这次穷举没有可以永久化的结果，你输入的存在时间范围为: %d 到 %d \n\n", startNum, endNum);
            if (isSaveOrNo) {
                file << "本次穷举没有可以永久化的结果:("<< endl;
                file.close();
            }
        }
        else {
            printf("这次穷举有可以永久化的结果，结果数量为:%d，你输入的存在时间范围为: %d 到 %d \n\n", HasAnw, startNum, endNum);
            if (isSaveOrNo) {
                file << "共计" << HasAnw << "个结果" << endl;
                file.close();
            }
        }
        closeNum = getModMax("如果要继续计算请输入y, 退出输入n:",0,1);
        if (closeNum) {
            break;
        }
    }
    return 0;
}
