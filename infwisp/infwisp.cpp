#include <iostream>
#include <string>
using std::string;

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
        if (isNumber(stringNum)) {
            return std::stoi(stringNum);//获取正确的整数
        }
        else
        {
            printf("参数不为纯数字，请重新输入！\n");
        }
    }
}

int getModMax(const char* speaky,int modMax,int outInt = 0) { //自定义一个询问并获取数字的函数
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
    printf("永久法术计算工具 cpp重写版\n\n注:乒乓回弹和盘旋魔弹影响的存在时间数值一样\n\n");
    int startNum,endNum,modMax,addLT,pinLT,decLT, helLT,arcLT,splLT,YouNeedNum,closeNum = 0;
    while (true)
    {
        startNum = getNumber("输入投射物存在时间范围的起始值:");
        while (true){
            endNum = getNumber("输入投射物存在时间范围的终止值:");
            if (startNum <= endNum) { //判断终止值不能小于起始值
                break;
            }
            else
            {
                printf("终止值不能小于起始值。\n");
            }
        }
        modMax = getNumber("输入所有影响存在时间修正的上限数:");
        addLT = getModMax("如果不需要计算蓝表输入n,需要计算输入y:", modMax);//根据你所输入的改变可循环的最大值，以此实现无需计算
        pinLT = getModMax("如果不需要计算乒乓输入n,需要计算输入y:", modMax);
        decLT = getModMax("如果不需要计算红表输入n,需要计算输入y:", modMax);
        helLT = getModMax("如果不需要计算螺旋魔弹输入n,需要计算输入y:", modMax);
        arcLT = getModMax("如果不需要计算相位输入n,需要计算输入y:", modMax);
        splLT = getModMax("如果不需要计算连锁输入n,需要计算输入y:", modMax);
        for (int calNum = startNum; calNum <= endNum; calNum++) {//穷举计算
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
                                    YouNeedNum = (75*add+25*pin+50*hel+80*arc-dec*42-spl*30) + calNum;
                                    if (YouNeedNum == -1) {
                                        printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d \n",add,pin,hel,arc,dec,spl);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        printf("如果你没看到对应数量的话，那么代表没有结果，你输入的存在时间范围为: %d 到 %d \n\n", startNum, endNum);
        closeNum = getModMax("如果要继续计算请输入y, 退出输入n:",0,1);
        if (closeNum) {
            break;
        }
    }
    return 0;
}