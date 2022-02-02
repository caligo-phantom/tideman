#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float r = image[i][j].rgbtRed;
            float g = image[i][j].rgbtGreen;
            float b = image[i][j].rgbtBlue;

            int avg = round((r + b + g) / 3);

            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // creating a temporary array
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    // looping through rows
    for (int i = 0; i < height; i++)
    {
        // looping though columns
        for (int j = 0; j < width; j++)
        {
            float sum_r;
            float sum_g;
            float sum_b;
            int count;

            // initialize values
            sum_r = sum_g = sum_b = count = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // to check if px is outside rows
                    if (i + k < 0 || i + k >= height)
                    {
                        continue;
                    }
                    // to check if px is outside columns
                    if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }
                    // else add to sums
                    sum_r += tmp[i + k][j + l].rgbtRed;
                    sum_g += tmp[i + k][j + l].rgbtGreen;
                    sum_b += tmp[i + k][j + l].rgbtBlue;
                    count++;
                }
            }
            // get avg and blur img
            image[i][j].rgbtRed = round(sum_r / count);
            image[i][j].rgbtGreen = round(sum_g / count);
            image[i][j].rgbtBlue = round(sum_b / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // creating a temporary array
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    // initialize sobel arrays
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // looping though rows
    for (int i = 0; i < height; i++)
    {
        // looping though columns
        for (int j = 0; j < width; j++)
        {
            float gx_red;
            float gx_green;
            float gx_blue;

            float gy_red;
            float gy_green;
            float gy_blue;

            gx_red = gx_blue = gx_green = gy_red = gy_blue = gy_green = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // check if px is outside rows
                    if (i + k < 0 || i + k >= height)
                    {
                        continue;
                    }

                    // check if px is outside columns
                    if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }

                    // else add to sums
                    gx_red += tmp[i + k][j + l].rgbtRed * gx[k + 1][l + 1];
                    gx_green += tmp[i + k][j + l].rgbtGreen * gx[k + 1][l + 1];
                    gx_blue += tmp[i + k][j + l].rgbtBlue * gx[k + 1][l + 1];

                    gy_red += tmp[i + k][j + l].rgbtRed * gy[k + 1][l + 1];
                    gy_green += tmp[i + k][j + l].rgbtGreen * gy[k + 1][l + 1];
                    gy_blue += tmp[i + k][j + l].rgbtBlue * gy[k + 1][l + 1];
                }
            }

            int red = round(sqrt(gx_red * gx_red + gy_red * gy_red));
            int green = round(sqrt(gx_green * gx_green + gy_green * gy_green));
            int blue = round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));

            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            // assign new values to px
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}