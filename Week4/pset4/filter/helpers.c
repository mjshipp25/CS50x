#include "helpers.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            BYTE new_pixel = round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);
            image[i][j].rgbtBlue = new_pixel;
            image[i][j].rgbtGreen = new_pixel;
            image[i][j].rgbtRed = new_pixel;
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
            RGBTRIPLE buffer = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // creates a temporary array to do calculations onto
    RGBTRIPLE temp[height][width]; // tempory array
    for (int i =  0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j]; // temp array is a duplicate of the stock image
        }
    }

    // blurs image
    for (int i =  0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            bool iLatch = false;

            int counter = 0; // counter number of pixels added
            int sumBlue = 0; // sum of the blue pixels
            int sumGreen = 0; // sum of the green pixels
            int sumRed = 0; // sum of the red pixels

            // sums the values in the 3x3 box
            for (int k = -1, m = 2; k < m; k++)
            {
                // checks if pixel is on the top corner
                if (i == 0 && iLatch == false)
                {
                    k++;
                    iLatch = true;
                }

                // checks if pixel is on bottom ledge
                if (i == height - 1 && iLatch == false)
                {
                    iLatch = true;
                    m--;
                }

                // sets latch to false before loop
                bool jLatch = false;

                for (int l = -1, n = 2; l < n; l++)
                {
                    // checks if pixel is on the left or edge
                    if (j == 0 && jLatch == false)
                    {
                        l++;
                        jLatch = true;
                    }

                    // checks if pixel is on the right or edge
                    if (j == width - 1 && jLatch == false)
                    {
                        jLatch = true;
                        n--;

                    }
                    // sums the 3x3 box
                    sumBlue += temp[i + k][j + l].rgbtBlue;
                    sumGreen += temp[i + k][j + l].rgbtGreen;
                    sumRed += temp[i + k][j + l].rgbtRed;

                    // counts how many boxes are added
                    counter++;
                }
            }
            // calculates the avg of 3x3 box and assignes it to pixel in image
            image[i][j].rgbtBlue = round((float)sumBlue / counter);
            image[i][j].rgbtGreen = round((float)sumGreen / counter);
            image[i][j].rgbtRed = round((float)sumRed / counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width]; // tempory array
    for (int i =  0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j]; // temp array is a duplicate of the stock image
        }
    }

    // arrays of Sobel factors
    int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int i = 0; i < height; i++) // check for height
    {
        for (int j = 0; j < width; j++) // check for width
        {
            int index = 0; // index over Gx and Gy array

            int sumGxBlue = 0; // sum of Gx for blue pixels
            int sumGxGreen = 0; // sum of Gx for green pixels
            int sumGxRed = 0; // sum of Gx for red pixels

            int sumGyBlue = 0; // sum of Gy for blue pixels
            int sumGyGreen = 0; // sum of Gy for green pixels
            int sumGyRed = 0; // sum of Gy for red pixels

            for (int k = -1, m = 2; k < m; k++)
            {
                // checks if pixel is in the corner or edge
                if (i + k < 0 || i + k > height - 1)
                {
                    index = index + 3;
                    continue;
                }

                for (int l = -1, n = 2; l < n; l++)
                {
                    // checks if pixel is in the corner or edge
                    if (j + l < 0 || j + l > width - 1)
                    {
                        index++;
                        continue;
                    }

                    // adds Sobel factor times the pixel to the sum
                    sumGxBlue += (Gx[index] * temp[i + k][j + l].rgbtBlue);
                    sumGyBlue += (Gy[index] * temp[i + k][j + l].rgbtBlue);

                    sumGxGreen += (Gx[index] * temp[i + k][j + l].rgbtGreen);
                    sumGyGreen += (Gy[index] * temp[i + k][j + l].rgbtGreen);

                    sumGxRed += (Gx[index] * temp[i + k][j + l].rgbtRed);
                    sumGyRed += (Gy[index] * temp[i + k][j + l].rgbtRed);

                    index++;
                }
            }
            // uses sums and Sobel formula to calculate new pixel color
            int sumBlue = round(sqrt((sumGxBlue * sumGxBlue) + (sumGyBlue * sumGyBlue)));
            int sumGreen = round(sqrt((sumGxGreen * sumGxGreen) + (sumGyGreen * sumGyGreen)));
            int sumRed = round(sqrt((sumGxRed * sumGxRed) + (sumGyRed * sumGyRed)));

            // checks if new pixel color is larger than 255 (int rang)
            if (sumBlue > 255)
            {
                image[i][j].rgbtBlue = 255; // if so then sets it to 255
            }
            else
            {
                image[i][j].rgbtBlue = sumBlue;
            }

            if (sumGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sumGreen;
            }

            if (sumRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sumRed;
            }
        }
    }
    return;
}