#include <output_writer.hpp>

namespace output_writer {
  void OutWriter::write(bool bit)
  {
    if(bit)
      bit_buffer |= (1 << bit_idx);
    else
      bit_buffer &= ~(1 << bit_idx);
    if(bit_idx == 0) {
      out_stream.write(reinterpret_cast<const char*>(&bit_buffer),
                       sizeof(bit_buffer));
      bit_idx = 7;
      bit_buffer = 0;
    } else
      bit_idx--;
  }

  void OutWriter::flush_buffer()
  {
    while(bit_idx != 7)
      write(false); // add 0 to buffer
  }

  OutWriter::~OutWriter()
  {
    flush_buffer();
    out_stream.close();
  }
} // namespace output_writer