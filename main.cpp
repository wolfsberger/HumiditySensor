#include "stdio.h"
#include "mbed.h"
#include "HygroClip2.h"
#include "SPI_TFT_ILI9341.h"
#include "Arial12x12.h"
#include "Arial28x28.h"
#include "GraphScale.h"
#include "LineGraph.h"
#include "CommandHandler.h"


Serial pc(SERIAL_TX, SERIAL_RX);
HygroClip2 sensor(D8,D2);
SPI_TFT_ILI9341 TFT(SPI_MOSI,SPI_MISO,SPI_SCK,SPI_CS,D9,D7);

const int ColorTemperature = RGB(0xff,0x37,0x00);
const int ColorHumidity = RGB(0x00,0xc8,0xff);
const int ColorDewPoint = RGB(0x00,0xE8,0x59);
const int ColorHumidityAbs = RGB(0xff,0x44,0xa5);
const int GraphUpdateRateInSeconds = 1;

uint8_t currentGraph = 0;

bool nextGraph(uint32_t);

LineGraph<200> graphTemperature(&TFT,1,200,200,100,0,100);
LineGraph<200> graphHumidity(&TFT,1,200,200,100,0,100);
LineGraph<200> graphDewPoint(&TFT,1,200,200,100,0,100);

class TEST
{
public:
    bool run(CommandBase::ParameterType param)
    {
        return param < 5;
    }
};

TEST X;

CommandHandler commandHandler;
FunctionCommand commandNextGraph(0x0001, nextGraph);
MemberCommand<TEST> commandTest(0x0002, &X, &TEST::run);

void drawChangingValues()
{
    TFT.set_font((unsigned char*)Arial28x28);
    TFT.locate(10,25);
    TFT.foreground(ColorTemperature);
    TFT.printf("%.1f*C ",sensor.getTemperature()); // * will be displayed as °
    TFT.locate(10,70);
    TFT.foreground(ColorHumidity);
    TFT.printf("%.1f%%rH ",sensor.getHumidity());
    TFT.foreground(ColorHumidityAbs);
    TFT.locate(10,115);
    TFT.printf("%.3fg/m3 ",sensor.getAbsolutHumidity());
    TFT.foreground(ColorDewPoint);
    TFT.locate(10,160);
    TFT.printf("%.1f*C ",sensor.getDewPoint());    // * will be displayed as °
}

void drawGraphs()
{
    graphTemperature.draw(currentGraph == 0 ? ColorTemperature : Black);
    graphHumidity.draw(currentGraph == 1 ? ColorHumidity : Black);
    graphDewPoint.draw(currentGraph == 2 ? ColorDewPoint : Black);
}

void handleUART()
{
    uint8_t c = pc.getc();
    commandHandler.handleCommand(c,0x00000000);
}

bool nextGraph(uint32_t arg)
{
    currentGraph = (currentGraph+1) % 3;
    drawGraphs();
    return true;
}

int main()
{
    commandHandler.addCommand(&commandNextGraph);
    commandHandler.addCommand(&commandTest);

    pc.baud(57600);
    pc.attach(handleUART);

    TFT.claim(stdout);      // send stdout to the TFT display
    TFT.background(Black);    // set background to black
    TFT.foreground(White);    // set chars to white
    TFT.cls();                // clear the screen
    TFT.set_font((unsigned char*) Arial12x12);  // select the font
    TFT.set_orientation(2);

    TFT.locate(10,10);
    TFT.printf("Loading...");

    while(!sensor.isDataValid())
    {
        sensor.update();
        wait(.5f);
    }

    TFT.locate(10,10);
    TFT.printf("Temperatur");
    TFT.locate(10,55);
    TFT.printf("Luftfeuchtigkeit (relativ)");
    TFT.locate(10,100);
    TFT.printf("Luftfeuchtigkeit (absolut)");
    TFT.locate(10,145);
    TFT.printf("Taupunkt");

    GraphScale scale(&TFT,1,200,200,100,0,100,20);
    scale.draw(White);

    set_time(0);

    while(1) {

        sensor.update();

        drawChangingValues();

        time_t seconds = time(NULL);
        if (seconds >= GraphUpdateRateInSeconds)
        {
            set_time(0);
            graphTemperature.addItem(sensor.getTemperature());
            graphHumidity.addItem(sensor.getHumidity());
            graphDewPoint.addItem(sensor.getDewPoint());
            drawGraphs();
        }

        wait(1);
    }
}
