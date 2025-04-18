#include "drawer.h"

void draw(Data* data,  BoxStatus* items_status){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_SAMPLES, 8);
    glEnable(GL_MULTISAMPLE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGH, "2DOPP Visualization", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGH);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho(0.0,WINDOW_WIDTH, 0.0, WINDOW_HEIGH, 0.0, 1.0);


    typedef struct{
        double x;
        double y;
    }Point2D;

    const double scale = WINDOW_WIDTH/(5*data->width);

    Point2D stockSheetPos;
    Point2D remainingItemsStartPos;

    stockSheetPos.x = WINDOW_WIDTH * 0.1;
    stockSheetPos.y = WINDOW_HEIGH / 4.0;

    remainingItemsStartPos.x = (stockSheetPos.x + data->width * scale) * 1.2;
    remainingItemsStartPos.y = WINDOW_HEIGH * 0.9; 

    while (!glfwWindowShouldClose(window))
    {

        /* Poll for and process events */
        glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(1.5);


        /*Stock-sheet*/
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(stockSheetPos.x, stockSheetPos.y);
        glVertex2f(stockSheetPos.x + data->width * scale, stockSheetPos.y);
        glEnd();

        /*Items*/
        int itemCounter = 0;
        int packedItemCounter = 0;
        double lastXCoord = 0;
        for(int i = 0; i < data->nunique_items; i++){    

            BoxStatus item = items_status[i];

            glColor3f(colors[itemCounter][0], colors[itemCounter][1], colors[itemCounter][2]);
            double itemWidth, itemHeight;
            if(item.rotated){
                itemWidth = data->items[item.index - 1].height;
                itemHeight = data->items[item.index - 1].width;
            }else{
                itemWidth = data->items[item.index - 1].width;
                itemHeight = data->items[item.index - 1].height;
            }



            int x = stockSheetPos.x + item.x * scale;
            int y = stockSheetPos.y + item.y * scale;


            glColor3f(colors[packedItemCounter][0], colors[packedItemCounter][1], colors[packedItemCounter][2]);
            glRectf(x, y, x + itemWidth*scale, y + itemHeight*scale);
            packedItemCounter++;
            // continue;


            // double x = remainingItemsStartPos.x + lastXCoord + 30;
            // glRectf(x, remainingItemsStartPos.y, x + itemWidth * scale, remainingItemsStartPos.y + itemHeight*scale);
            // lastXCoord =  (x + itemWidth * scale) - remainingItemsStartPos.x;
            // itemCounter++;


            // if(remainingItemsStartPos.x + lastXCoord >= WINDOW_WIDTH * 0.8){
            //     remainingItemsStartPos.y -= WINDOW_HEIGH * 0.20;
            //     lastXCoord = 0;
            // }
        }


        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(stockSheetPos.x, stockSheetPos.y + data->height * scale);
        glVertex2f(stockSheetPos.x + data->width * scale, stockSheetPos.y + data->height * scale);

        glEnd();

        remainingItemsStartPos.y = WINDOW_HEIGH * 0.8; 




        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

}