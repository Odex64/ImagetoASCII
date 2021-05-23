#include <iostream>
#include <fstream>
#include <SFML/Graphics/Image.hpp>
#include "HSL.hpp"

std::wstring WriteChar(float l, unsigned int mode)
{
    switch (mode)
    {
    case 1:
        if (l >= 0.95)
            return L"  ";
        else if (l >= 0.7 && l < 0.95)
            return L"==";
        else if (l >= 0.4 && l < 0.7)
            return L"++";
        else if (l >= 0.1 && l < 0.4)
            return L"%:";
        else if (l < 0.1)
            return L"@.";
        return NULL;

    case 2:
        if (l >= 0.95)
            return L"  ";
        else if (l >= 0.7 && l < 0.95)
            return L"░░";
        else if (l >= 0.4 && l < 0.7)
            return L"▒▒";
        else if (l >= 0.1 && l < 0.4)
            return L"▓▓";
        else if (l < 0.1)
            return L"██";
        return NULL;
    }
}

int FindLastIndex(std::string str, char x)
{
    int index = -1;
    for (unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] == x)
            index = i;
    }
    return index;
}

void ConvertToASCII(sf::Image image, std::string path, unsigned int mode)
{
    mode = mode > 0 && mode <= 2 ? mode : 1;
    std::wofstream textFile(path.substr(0, FindLastIndex(path, '.')) + ".txt");
    for (unsigned int y = 0; y < image.getSize().y; y++)
    {
        for (unsigned int x = 0; x < image.getSize().x; x++)
        {
            sf::Color color = image.getPixel(x, y);
            textFile << WriteChar(HSL::RGBtoHSL(color.r, color.g, color.b).GetLuminance(), mode);
        }
        textFile << std::endl;
    }
}

sf::Image ResizeImage(sf::Image originalImage, unsigned int targetWidth)
{
    sf::Vector2u size = originalImage.getSize();
    sf::Vector2u newSize;

    int max_dim = size.x >= size.y ? size.x : size.y;
    float scale = ((float)targetWidth) / max_dim;

    if (size.x >= size.y)
    {
        newSize.x = targetWidth;
        newSize.y = size.y * scale;
    }
    else
    {
        newSize.y = targetWidth;
        newSize.x = size.x * scale;
    }

    sf::Image resizedImage;
    resizedImage.create(newSize.x, newSize.y);

    for (unsigned int y = 0; y < newSize.y; y++)
    {
        for (unsigned int x = 0; x < newSize.x; x++)
        {
            unsigned int origX = static_cast<unsigned int>(static_cast<double>(x) / newSize.x * originalImage.getSize().x);
            unsigned int origY = static_cast<unsigned int>(static_cast<double>(y) / newSize.y * originalImage.getSize().y);
            resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
        }
    }
    return resizedImage;
}

bool LoadFromPath(std::string path)
{
    sf::Image image;
    unsigned int width;
    if (image.loadFromFile(path))
    {
        std::cout << "Image size: " << image.getSize().x << ' ' << image.getSize().y << std::endl;
        do {
            std::cout << "Enter target size (type 0 for original): ";
            std::cin >> width;
        } while (width > image.getSize().x);

        if (width != 0)
            image = ResizeImage(image, width);
        // image.saveToFile(path.substr(0, FindLastIndex(path, '.')) + "temp.png");

        unsigned int mode;
        std::cout << "Legacy(1), UNICODE(2): ";
        std::cin >> mode;
        ConvertToASCII(image, path, mode);

        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::locale::global(std::locale(""));
    std::string path;

    if (argc == 2 && LoadFromPath(std::string(argv[1])))
        return 0;
    else
    {
        do {
            std::cout << "Type file path (white background recommended): ";
            std::cin >> path;
        } while (!LoadFromPath(path));
    }

    return 0;
}