#include <output_writer.hpp>

namespace output_writer {
  void OutWriter::write(bool bit)
  {
      bit_buffer = (bit_buffer << 1) | bit;
      ++bit_count;

      if (bit_count == 8)
      {
          flush_buffer();
      }
  }

  void OutWriter::flush_buffer()
  {
      if (bit_count > 0) {
          // Pad remaining bits if needed
          bit_buffer <<= (8 - bit_count);
          out_stream.put(static_cast<char>(bit_buffer));
          bit_buffer = 0;
          bit_count = 0;
      }
  }

  OutWriter::~OutWriter()
  {
    flush_buffer();
    out_stream.close();
  }
} // namespace output_writer