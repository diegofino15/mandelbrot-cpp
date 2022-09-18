#include <raylib.h>
#include <iostream>
#include <math.h>


long double mapTo(long double n, long double minVal, long double maxVal, long double newMinVal, long double newMaxVal) {
    return ((n - minVal) / (maxVal - minVal)) * (newMaxVal - newMinVal) + newMinVal;
}

Color getValue(long double a, long double b) {
    int maxIterations = 500;

    long double ca = a;
    long double cb = b;

    int n = 0;

    long double mult = 2;
    
    long double aa, bb;
    while (n < maxIterations) {
        aa = a*a - b*b;
        bb = mult * a * b;

        a = aa + ca;
        b = bb + cb;

        if (abs(a + b) > 16) { break; }
        n++;
    }

    Color color;
    color.a = 255;

    color.r = ((int) (n * PI)) % 255;
    color.g = ((int) sqrt(n * PI)) % 255;
    color.b = ((int) (n * PI * sqrt(PI))) % 255;

    if (n == maxIterations) { color = BLACK; }

    return color;
}

void drawFractal(RenderTexture2D fractalTexture, float screenWidth, float screenHeight, long double minX, long double maxX, long double minY, long double maxY, float resolution) {
    BeginTextureMode(fractalTexture);

    for (float x = 0; x < screenWidth; x += resolution) {
        for (float y = 0; y < screenHeight; y += resolution) {
            long double a = mapTo(x, 0.0f, (float) screenWidth, minX, maxX);
            long double b = mapTo(y, 0.0f, (float) screenHeight, minY, maxY);

            Color color = getValue(a, b);
            DrawPixel(x, screenHeight - y, color);
        }
    }

    EndTextureMode();
}

int main()
{
    const int screenWidth = 700;
    const int screenHeight = 700;
    const float resolution = 1.0f;

    InitWindow(screenWidth, screenHeight, "Mandelbrot Fractal");
    SetTargetFPS(60);

    RenderTexture2D fractal = LoadRenderTexture(screenWidth, screenHeight);
    
    long double minX = -2.0f;
    long double minY = -2.0f;
    long double maxX = 2.0f;
    long double maxY = 2.0f;

    drawFractal(fractal, screenWidth, screenHeight, minX, maxX, minY, maxY, resolution);
    
    int msx1, msy1, msx2, msy2;
    bool isClicking = false;
    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(0)) {
            if (!isClicking) {
                msx1 = GetMouseX();
                msy1 = GetMouseY();
                isClicking = true;
            }
        } else {
            if (isClicking) {
                isClicking = false;

                msx2 = GetMouseX();
                msy2 = std::max(msx1, msx2) - std::min(msx1, msx2) + msy1;

                int minMsX = std::min(msx1, msx2);
                int minMsY = std::min(msy1, msy2);
                int maxMsX = std::max(msx1, msx2);
                int maxMsY = std::max(msy1, msy2);

                long double newMinX = mapTo((float) minMsX, 0.0f, (float) screenWidth, minX, maxX);
                long double newMinY = mapTo((float) minMsY, 0.0f, (float) screenHeight, minY, maxY);
                long double newMaxX = mapTo((float) maxMsX, 0.0f, (float) screenWidth, minX, maxX);
                long double newMaxY = mapTo((float) maxMsY, 0.0f, (float) screenHeight, minY, maxY);

                minX = newMinX;
                minY = newMinY;
                maxX = newMaxX;
                maxY = newMaxY;

                drawFractal(fractal, screenWidth, screenHeight, minX, maxX, minY, maxY, resolution);
            }
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                fractal.texture,
                {0.0f, 0.0f, (float) fractal.texture.width, (float) fractal.texture.height},
                {0.0f, 0.0f, screenWidth, screenHeight},
                {0.0f, 0.0f}, 0.0f, WHITE
            );
            
            long int scale = (long int) (4.0f / (maxX - minX));
            DrawText(std::to_string(scale).c_str(), 10, 10, 20, WHITE);

            if (isClicking) {
                int x2 = GetMouseX();
                int y2 = GetMouseY();
                DrawRectangleLines(std::min(msx1, x2), std::min(msy1, y2), std::max(msx1, x2) - std::min(msx1, x2), std::max(msx1, x2) - std::min(msx1, x2), WHITE);
            }

        EndDrawing();
    }
    
    UnloadRenderTexture(fractal);
    CloseWindow();
    return 0;
}



