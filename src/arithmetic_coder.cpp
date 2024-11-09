#include <arithmetic_coder.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

#define NO_SYMBOLS 256

static void insert_to_buffer(uint8_t& bit_buffer, size_t& bit_buff_idx, uint64_t& output_size, uint8_t bit, std::ofstream* output)
{
    if(bit)
        bit_buffer |= (1<<bit_buff_idx);
    else
        bit_buffer &= ~(1<<bit_buff_idx);
    if(bit_buff_idx == 0)
    {
        output->put(reinterpret_cast<char&>(bit_buffer));
        bit_buff_idx = 7;
        bit_buffer = 0;
        output_size++;
    }
    else
        bit_buff_idx--;
}

static uint8_t get_bit(uint8_t& byte, uint8_t& bit_index, std::ifstream* input, bool& eof)
{
    uint8_t bit = (byte >> bit_index) & 1;
    if(bit_index == 0)
    {
        input->get(reinterpret_cast<char&>(byte));
        eof = (input->peek() == EOF);
        bit_index = 7;
    } else bit_index--;
    return bit;
}


uint64_t Arithmetic_Coder::encode(std::ofstream* out_stream) {
    auto* occur = static_cast<uint64_t*>(calloc(NO_SYMBOLS, sizeof(uint64_t)));
    for(size_t i = 0; i < NO_SYMBOLS; i++)
        occur[i]++;
    uint64_t input_size = NO_SYMBOLS;
    uint64_t output_size = 0;

    // Encode
    const uint64_t precision = 32;
    const uint64_t whole = (uint64_t)1 << precision;
    const uint64_t half = whole / 2;
    const uint64_t quart = whole / 4;

    uint8_t bit_buffer = 0;         // Bit buffer
    size_t bit_buff_idx = 7;        // Index in buffer
    uint64_t l = 0;                 // Lower limit
    uint64_t u = whole;             // Upper limit
    int16_t scale = 0;              // Scale

    // ------------- MAIN LOOP ----------------

    uint8_t sym;
    while(in_stream->peek() != EOF)
    {
        in_stream->get(reinterpret_cast<char&>(sym));
        uint64_t cumulative_count = 0;
        for(uint16_t i = 0; i < sym; i++)
            cumulative_count += occur[i];

        // Update low and up
        auto dist = static_cast<double>(u - l);
        u = l + static_cast<uint64_t>(roundl(dist * (static_cast<double>(cumulative_count + occur[sym])/static_cast<double>(input_size))));
        l = l + static_cast<uint64_t>(roundl(dist * (static_cast<double>(cumulative_count)/static_cast<double>(input_size))));

        while(true)
        {
            if(u < half)
            {
                insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 0, out_stream);
                while(scale > 0)
                {
                    insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 1, out_stream);
                    scale--;
                }
            } else if(l > half){
                l -= half;
                u -= half;
                insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 1, out_stream);
                while(scale > 0)
                {
                    insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 0, out_stream);
                    scale--;
                }
            } else if(l > quart && u < 3*quart){
                l -= quart;
                u -= quart;
                scale++;
            } else
                break;
            u <<= 1;
            l <<= 1;
        }
        occur[sym]++;
        input_size++;
    }
    // EOF
    scale++;
    if( l <= quart )
    {
        insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 0, out_stream);
        while(scale > 0)
        {
            insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 1, out_stream);
            scale--;
        }
    } else{
        insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 1, out_stream);
        while(scale > 0)
        {
            insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 0, out_stream);
            scale--;
        }
    }
    // Flush buffer
    while (bit_buff_idx != 7)
        insert_to_buffer(bit_buffer, bit_buff_idx, output_size, 0, out_stream);

    free(occur);
    return output_size;
}

std::filesystem::path Arithmetic_Coder::decode()
{
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
    std::filesystem::path temp_filepath = temp_dir / "tempfile.txt";
    std::ofstream out_stream(temp_filepath);
    if(!out_stream.is_open()) {
        std::cerr << "Cannot open temp file: " << temp_filepath << std::endl;
    }

    const uint64_t precision = 32;
    const uint64_t whole = (uint64_t)1 << precision;
    const uint64_t half = whole / 2;
    const uint64_t quart = whole / 4;
    bool eof;

    auto* occur = static_cast<uint64_t*>(calloc(NO_SYMBOLS, sizeof(uint64_t)));
    for (size_t i = 0; i < NO_SYMBOLS; i++)
        occur[i]++;                     // Set occurrence of each symbol to 1
    uint64_t input_size = NO_SYMBOLS;   // Total number of symbols
    uint64_t z = 0;                     // "Znacznik"

    uint8_t byte;                       // Byte_buffer
    in_stream->get(reinterpret_cast<char&>(byte));
    uint8_t decoded_sym;                // Decoded symbol
    uint8_t bit_index = 7;              // Bit index in byte_buffer

    uint64_t l = 0;                     // Lower limit
    uint64_t u = whole;                 // Upper limit

    uint64_t i = 1;
    while(i <= precision && !eof)
    {
        if(get_bit(byte, bit_index, in_stream, eof) != 0)
            z +=  static_cast<uint64_t>(pow(2, static_cast<double>(precision - i)));
        i++;
    }

    // ------------- MAIN LOOP ----------------

    while(true){
        uint64_t cumulative_sum = 0;
        for(uint16_t sym = 0; sym < NO_SYMBOLS; sym++){
            auto dist = static_cast<double>(u - l);
            cumulative_sum += occur[sym];

            uint64_t new_u = l + static_cast<uint64_t>(roundl(dist * (static_cast<double>(cumulative_sum)/static_cast<double>(input_size))));
            uint64_t new_l = l + static_cast<uint64_t>(roundl(dist * (static_cast<double>(cumulative_sum - occur[sym])/static_cast<double>(input_size))));

            if(new_l <= z && z < new_u){
                decoded_sym = sym;
                input_size++;
                occur[sym]++;
                out_stream.put(reinterpret_cast<char&>(decoded_sym));
                l = new_l;
                u = new_u;
                break;
            }
        }

        if(eof)
            break;

        while(true){
            if(u < half){

            }else if(l > half){
                l -= half;
                u -= half;
                z -= half;
            }else if(l > quart && u < 3 * quart){
                l -= quart;
                u -= quart;
                z -= quart;
            }
            else
                break;

            l <<= 1;
            u <<= 1;
            z <<= 1;
            if(!eof && get_bit(byte, bit_index, in_stream, eof))
                z += 1;
        }
    }
    free(occur);

    return temp_filepath;
}