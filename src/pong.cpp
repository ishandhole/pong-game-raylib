#include <iostream>
#include <raylib.h>
using namespace std;

int pscore = 0;
int cscore = 0;

class Ball{
    public:
    float x,y;
    int speedX,speedY;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius,WHITE);
    }

    void update(){
        x += speedX;
        y += speedY;

        if(y+radius >= GetScreenHeight() || y-radius<=0){
            speedY *= -1;
        }
        if(x+radius >= GetScreenWidth()){
            cscore++;
            Reset();
        }
        if(x-radius <= 0){
            pscore++;
            Reset();
        }
    }   

    void Reset(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int schoices[2] = {-1,1};
        speedX *= schoices[GetRandomValue(0,1)]; 
        speedY *= schoices[GetRandomValue(0,1)];
    }
}ball;

class Padle{

    void LimitMovement(){
        if(y+height>=GetScreenHeight()){
            y=GetScreenHeight()-height;
        }
        else if(y<=0){
            y=0;
        }
    }

    public:
    float x,y;
    float width,height;
    int speed;

    void Draw(){
        DrawRectangle(x,y,width,height,WHITE);
    }

    void update(){
        if(IsKeyDown(KEY_DOWN)){
            y +=speed;
        }
        if(IsKeyDown(KEY_UP)){
            y -=speed;
        }
        
        LimitMovement();
    }

}paddle;

class CPaddle:public Padle{
    public:

    void update(int ball_y){
        if(y+height/2 >ball_y){
            y-=speed;
        }
        if(y+height<=ball_y){
            y+=speed;
        }
        if(y+height>=GetScreenHeight()){
            y=GetScreenHeight()-height;
        }
        else if(y<=0){
            y=0;
        }
    }

}cpaddle;

int main(){
    const int swidth = 1200;
    const int sheight = 800;

    InitWindow(swidth,sheight,"Pong game");
    SetTargetFPS(60);

    ball.x = swidth/2;
    ball.y = sheight/2;
    ball.radius = 20;
    ball.speedX = 7;
    ball.speedY = 7;

    paddle.width = 25;
    paddle.height = 120;
    paddle.x = swidth-35;
    paddle.y = sheight/2-60;
    paddle.speed = 6;

    cpaddle.width = 20;
    cpaddle.height = 120;
    cpaddle.x = 10;
    cpaddle.y = sheight/2-60;
    cpaddle.speed = 6;

    while (WindowShouldClose()==false){

        //Motion
        ball.update();
        paddle.update();
        cpaddle.update(ball.y);
        
        //Collision
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y},ball.radius, Rectangle{ paddle.x,paddle.y,paddle.width,paddle.height})){
            ball.speedX *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y},ball.radius, Rectangle{cpaddle.x, cpaddle.y, cpaddle.width, cpaddle.height})){
            ball.speedX *= -1;
        }

        //Drawing
        ClearBackground(BLACK);
        BeginDrawing();
        DrawLine(swidth/2,0,swidth/2,sheight,WHITE);
        ball.Draw();
        cpaddle.Draw();
        paddle.Draw();
        DrawText(TextFormat("%i",cscore), swidth/4-20, 20, 30, WHITE);
        DrawText(TextFormat("%i",pscore), 3*swidth/4-20, 20, 30,WHITE);
        EndDrawing();
    }
    CloseWindow();
}