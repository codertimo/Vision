//
// Created by 김준성 on 2016. 5. 14..
//

#include "android_core.h"

static int linesize = 30;
static int w1 =120;
static int frame_count =1;
static LineCounts linecounts;
static bool isInitialize = false;

int a;
int b;

int cornerDetection(Mat origin) {

    if(!isInitialize)
    {
        escapeDetection_initailize();
        isInitialize = true;
    }

    Mat dst;

    resize(origin, dst, Size(origin.cols*0.3,origin.rows*0.3));

    //Houghline Weight 동적 조정
    houghline_stablization_android();

    //Houghline 으로 Line 검출 및 분류
    ResultLines resultLines = lineDetection(dst,w1,0);

    //Line 누적 <40Frame>
    linecounts.insert(resultLines);
    escapeDetection_lineAccumulate(resultLines);

    //40Frame에 한번 검사
    if(frame_count++%40==0)
    {
        //방향성 검출
        int direction_code = cornerDetection2(linecounts);

        //보도이탈 검출
        int escape_code = escapeDetection();

        //누적된 line들 초기화
        linecounts.clear();

        //검출결과 반환
        return direction_code*10+escape_code;
    }

    return NOTHING;
}

void houghline_stablization_android() {
    //line크기에 따른 HoughLine Weight값 조정
    if(linesize>20)
        w1 += 10;
    if(linesize > 50)
        w1 += 10;
    if(linesize<10)
        w1 -= 10;
}
void setLineSize(int size) {
    linesize = size;
}

string toString(int code)
{
    switch (code)
    {
        case RIGHT_VERTICAL:
            return "Right Vertical Corner";
        case RIGHT_ROUND:
            return "Right Bended Corner";
        case LEFT_VERTIVAL:
            return "Left Vertical Corner";
        case LEFT_ROUND:
            return "Left Round Corner";
    }
    return "Nothing";
}

