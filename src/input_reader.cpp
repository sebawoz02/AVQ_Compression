#include <input_reader.hpp>

#include <iostream>
#include <header_tga.hpp>

namespace input_reader{

    static Image load_image(std::ifstream* input_stream,
                                                      TGA_header tga_head);
    static bool ends_with(const std::string& full_string, const std::string& ending);

    Image load_image(std::ifstream* input_stream,
                                               TGA_header tga_head)
    {
        Image image(tga_head.height,std::vector<Pixel>(tga_head.width));

        for(int i = 0; i < tga_head.height; i++) {
            for(int j = 0; j < tga_head.width; j++) {
                *input_stream >> std::noskipws >> image[i][j].blue;
                *input_stream >> std::noskipws >> image[i][j].green;
                *input_stream >> std::noskipws >> image[i][j].red;
            }
        }

        return image;
    }

    static bool ends_with(const std::string& full_string, const std::string& ending)
    {
        if(ending.size() > full_string.size())
            return false;

        return full_string.compare(full_string.size() - ending.size(), ending.size(), ending) == 0;
    }

    InReader::InReader(char* input_name)
    {
        std::string filename = std::string(input_name);
        if(ends_with(filename, ".tga"))
        {
            mode = "tga";
            std::ifstream input_stream = std::ifstream(filename);
            if(!input_stream.is_open())
            {
                std::cerr << "Cannot open file: " << filename << std::endl;
            }

            // Read header
            TGA_header header{};
            input_stream.read(reinterpret_cast<char*>(&header), sizeof(TGA_header));

            // Load Pixels to 2D vector
            image = load_image(&input_stream, header);

            input_stream.close();
        }
        else if(ends_with(filename, ".png"))
        {
            mode = "png";
        }
        else {
            mode = "unknown";
            std::cerr << "Cannot read input image. Unknown filetype." << std::endl;
        }

    }

    Image InReader::get_image() {
        return image;
    }
}
