#include <systemc>
#include <list>
#include <map>
#include <vector>
#include <iostream>
#include "hls_stream.h"
#include "ap_int.h"
#include "ap_fixed.h"
using namespace std;
using namespace sc_dt;

namespace bcsim
{
  struct Buffer {
    char *first;
    Buffer(char *addr) : first(addr)
    {
    }
  };

  struct DBuffer : public Buffer {
    size_t ufree;

    DBuffer(size_t usize) : Buffer(nullptr), ufree(1<<10)
    {
      first = new char[usize*ufree];
    }

    ~DBuffer()
    {
      delete[] first;
    }
  };

  struct CStream {
    char *front;
    char *back;
    size_t num;
    size_t usize;
    std::list<Buffer*> bufs;
    bool dynamic;

    CStream() : front(nullptr), back(nullptr),
                num(0), usize(0), dynamic(true)
    {
    }

    ~CStream()
    {
      for (Buffer *p : bufs) {
        delete p;
      }
    }

    template<typename T>
    T* data()
    {
      return (T*)front;
    }

    template<typename T>
    void transfer(hls::stream<T> *param)
    {
      while (!empty()) {
        param->write(*(T*)nextRead());
      }
    }

    bool empty();
    char* nextRead();
    char* nextWrite();
  };

  bool CStream::empty()
  {
    return num == 0;
  }

  char* CStream::nextRead()
  {
    assert(num > 0);
    char *res = front;
    front += usize;
    --num;
    return res;
  }

  char* CStream::nextWrite()
  {
    if (dynamic) {
      if (static_cast<DBuffer*>(bufs.back())->ufree == 0) {
        bufs.push_back(new DBuffer(usize));
        back = bufs.back()->first;
      }
      --static_cast<DBuffer*>(bufs.back())->ufree;
    }
    char *res = back;
    back += usize;
    ++num;
    return res;
  }

  std::list<CStream> streams;
  std::map<char*, CStream*> prebuilt;

  CStream* createStream(size_t usize)
  {
    streams.emplace_front();
    CStream &s = streams.front();
    {
      s.dynamic = true;
      s.bufs.push_back(new DBuffer(usize));
      s.front = s.bufs.back()->first;
      s.back = s.front;
      s.num = 0;
      s.usize = usize;
    }
    return &s;
  }

  template<typename T>
  CStream* createStream(hls::stream<T> *param)
  {
    CStream *s = createStream(sizeof(T));
    {
      s->dynamic = true;
      while (!param->empty()) {
        T data = param->read();
        memcpy(s->nextWrite(), (char*)&data, sizeof(T));
      }
      prebuilt[s->front] = s;
    }
    return s;
  }

  template<typename T>
  CStream* createStream(T *param, size_t usize)
  {
    streams.emplace_front();
    CStream &s = streams.front();
    {
      s.dynamic = false;
      s.bufs.push_back(new Buffer((char*)param));
      s.front = s.back = s.bufs.back()->first;
      s.usize = usize;
      s.num = ~0UL;
    }
    prebuilt[s.front] = &s;
    return &s;
  }

  CStream* findStream(char *buf)
  {
    return prebuilt.at(buf);
  }
}
class AESL_RUNTIME_BC {
  public:
    AESL_RUNTIME_BC(const char* name) {
      file_token.open( name);
      if (!file_token.good()) {
        cout << "Failed to open tv file " << name << endl;
        exit (1);
      }
      file_token >> mName;//[[[runtime]]]
    }
    ~AESL_RUNTIME_BC() {
      file_token.close();
    }
    int read_size () {
      int size = 0;
      file_token >> mName;//[[transaction]]
      file_token >> mName;//transaction number
      file_token >> mName;//pop_size
      size = atoi(mName.c_str());
      file_token >> mName;//[[/transaction]]
      return size;
    }
  public:
    fstream file_token;
    string mName;
};
struct __cosim_s4__ { char data[4]; };
struct __cosim_s1__ { char data[1]; };
struct __cosim_s2__ { char data[2]; };
extern "C" void mmult_top(int*, int*, int*, long long*, short*, short*, short*, short*, short*, int*, int*, int*, int*, int*, int*, int*, int*, short*, short*, short*, short*, int*, int*, int*, int*, int*, int*, int*, int*, short*, short*, short*, short*, char, int, int, int, __cosim_s4__, int, __cosim_s1__, __cosim_s1__, __cosim_s1__, __cosim_s1__, __cosim_s1__, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
extern "C" void apatb_mmult_top_hw(char __xlx_apatb_param_gemm_mode, volatile void * __xlx_apatb_param_quantized_multiplier, volatile void * __xlx_apatb_param_shift, volatile void * __xlx_apatb_param_bias, __cosim_s4__* __xlx_apatb_param_bias_count, volatile void * __xlx_apatb_param_profiling, __cosim_s1__* __xlx_apatb_param_zero_point_lhs, __cosim_s1__* __xlx_apatb_param_zero_point_rhs, __cosim_s1__* __xlx_apatb_param_zero_point_dst, __cosim_s1__* __xlx_apatb_param_clamp_max, __cosim_s1__* __xlx_apatb_param_clamp_min, int __xlx_apatb_param_N_adj, int __xlx_apatb_param_M_adj, int __xlx_apatb_param_M_fea, int __xlx_apatb_param_P_w, volatile void * __xlx_apatb_param_B, volatile void * __xlx_apatb_param_D1, volatile void * __xlx_apatb_param_D2, volatile void * __xlx_apatb_param_D3, volatile void * __xlx_apatb_param_D4, int __xlx_apatb_param_array_c_adjust, volatile void * __xlx_apatb_param_rowPtr_fea1, volatile void * __xlx_apatb_param_rowPtr_fea2, volatile void * __xlx_apatb_param_rowPtr_fea3, volatile void * __xlx_apatb_param_rowPtr_fea4, volatile void * __xlx_apatb_param_columnIndex_fea1, volatile void * __xlx_apatb_param_columnIndex_fea2, volatile void * __xlx_apatb_param_columnIndex_fea3, volatile void * __xlx_apatb_param_columnIndex_fea4, volatile void * __xlx_apatb_param_values_fea1, volatile void * __xlx_apatb_param_values_fea2, volatile void * __xlx_apatb_param_values_fea3, volatile void * __xlx_apatb_param_values_fea4, volatile void * __xlx_apatb_param_rowPtr_adj1, volatile void * __xlx_apatb_param_rowPtr_adj2, volatile void * __xlx_apatb_param_rowPtr_adj3, volatile void * __xlx_apatb_param_rowPtr_adj4, volatile void * __xlx_apatb_param_columnIndex_adj1, volatile void * __xlx_apatb_param_columnIndex_adj2, volatile void * __xlx_apatb_param_columnIndex_adj3, volatile void * __xlx_apatb_param_columnIndex_adj4, volatile void * __xlx_apatb_param_values_adj1, volatile void * __xlx_apatb_param_values_adj2, volatile void * __xlx_apatb_param_values_adj3, volatile void * __xlx_apatb_param_values_adj4) {
  // Collect __xlx_quantized_multiplier__tmp_vec
  vector<sc_bv<32> >__xlx_quantized_multiplier__tmp_vec;
  for (int j = 0, e = 1024; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_quantized_multiplier)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_quantized_multiplier)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_quantized_multiplier)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_quantized_multiplier)[j*4+3];
    __xlx_quantized_multiplier__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_quantized_multiplier = 1024;
  int __xlx_offset_param_quantized_multiplier = 0;
  int __xlx_offset_byte_param_quantized_multiplier = 0*4;
  int* __xlx_quantized_multiplier__input_buffer= new int[__xlx_quantized_multiplier__tmp_vec.size()];
  for (int i = 0; i < __xlx_quantized_multiplier__tmp_vec.size(); ++i) {
    __xlx_quantized_multiplier__input_buffer[i] = __xlx_quantized_multiplier__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_shift__tmp_vec
  vector<sc_bv<32> >__xlx_shift__tmp_vec;
  for (int j = 0, e = 1024; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_shift)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_shift)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_shift)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_shift)[j*4+3];
    __xlx_shift__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_shift = 1024;
  int __xlx_offset_param_shift = 0;
  int __xlx_offset_byte_param_shift = 0*4;
  int* __xlx_shift__input_buffer= new int[__xlx_shift__tmp_vec.size()];
  for (int i = 0; i < __xlx_shift__tmp_vec.size(); ++i) {
    __xlx_shift__input_buffer[i] = __xlx_shift__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_bias__tmp_vec
  vector<sc_bv<32> >__xlx_bias__tmp_vec;
  for (int j = 0, e = 1024; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_bias)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_bias)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_bias)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_bias)[j*4+3];
    __xlx_bias__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_bias = 1024;
  int __xlx_offset_param_bias = 0;
  int __xlx_offset_byte_param_bias = 0*4;
  int* __xlx_bias__input_buffer= new int[__xlx_bias__tmp_vec.size()];
  for (int i = 0; i < __xlx_bias__tmp_vec.size(); ++i) {
    __xlx_bias__input_buffer[i] = __xlx_bias__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_profiling__tmp_vec
  vector<sc_bv<64> >__xlx_profiling__tmp_vec;
  for (int j = 0, e = 16; j != e; ++j) {
    sc_bv<64> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_profiling)[j*8+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_profiling)[j*8+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_profiling)[j*8+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_profiling)[j*8+3];
    _xlx_tmp_sc.range(39, 32) = ((char*)__xlx_apatb_param_profiling)[j*8+4];
    _xlx_tmp_sc.range(47, 40) = ((char*)__xlx_apatb_param_profiling)[j*8+5];
    _xlx_tmp_sc.range(55, 48) = ((char*)__xlx_apatb_param_profiling)[j*8+6];
    _xlx_tmp_sc.range(63, 56) = ((char*)__xlx_apatb_param_profiling)[j*8+7];
    __xlx_profiling__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_profiling = 16;
  int __xlx_offset_param_profiling = 0;
  int __xlx_offset_byte_param_profiling = 0*8;
  long long* __xlx_profiling__input_buffer= new long long[__xlx_profiling__tmp_vec.size()];
  for (int i = 0; i < __xlx_profiling__tmp_vec.size(); ++i) {
    __xlx_profiling__input_buffer[i] = __xlx_profiling__tmp_vec[i].range(63, 0).to_uint64();
  }
  // Collect __xlx_B__tmp_vec
  vector<sc_bv<16> >__xlx_B__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_B)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_B)[j*2+1];
    __xlx_B__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_B = 128000;
  int __xlx_offset_param_B = 0;
  int __xlx_offset_byte_param_B = 0*2;
  short* __xlx_B__input_buffer= new short[__xlx_B__tmp_vec.size()];
  for (int i = 0; i < __xlx_B__tmp_vec.size(); ++i) {
    __xlx_B__input_buffer[i] = __xlx_B__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_D1__tmp_vec
  vector<sc_bv<16> >__xlx_D1__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_D1)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_D1)[j*2+1];
    __xlx_D1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_D1 = 128000;
  int __xlx_offset_param_D1 = 0;
  int __xlx_offset_byte_param_D1 = 0*2;
  short* __xlx_D1__input_buffer= new short[__xlx_D1__tmp_vec.size()];
  for (int i = 0; i < __xlx_D1__tmp_vec.size(); ++i) {
    __xlx_D1__input_buffer[i] = __xlx_D1__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_D2__tmp_vec
  vector<sc_bv<16> >__xlx_D2__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_D2)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_D2)[j*2+1];
    __xlx_D2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_D2 = 128000;
  int __xlx_offset_param_D2 = 0;
  int __xlx_offset_byte_param_D2 = 0*2;
  short* __xlx_D2__input_buffer= new short[__xlx_D2__tmp_vec.size()];
  for (int i = 0; i < __xlx_D2__tmp_vec.size(); ++i) {
    __xlx_D2__input_buffer[i] = __xlx_D2__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_D3__tmp_vec
  vector<sc_bv<16> >__xlx_D3__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_D3)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_D3)[j*2+1];
    __xlx_D3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_D3 = 128000;
  int __xlx_offset_param_D3 = 0;
  int __xlx_offset_byte_param_D3 = 0*2;
  short* __xlx_D3__input_buffer= new short[__xlx_D3__tmp_vec.size()];
  for (int i = 0; i < __xlx_D3__tmp_vec.size(); ++i) {
    __xlx_D3__input_buffer[i] = __xlx_D3__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_D4__tmp_vec
  vector<sc_bv<16> >__xlx_D4__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_D4)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_D4)[j*2+1];
    __xlx_D4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_D4 = 128000;
  int __xlx_offset_param_D4 = 0;
  int __xlx_offset_byte_param_D4 = 0*2;
  short* __xlx_D4__input_buffer= new short[__xlx_D4__tmp_vec.size()];
  for (int i = 0; i < __xlx_D4__tmp_vec.size(); ++i) {
    __xlx_D4__input_buffer[i] = __xlx_D4__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_fea1__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_fea1__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_fea1)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_fea1)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_fea1)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_fea1)[j*4+3];
    __xlx_rowPtr_fea1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_fea1 = 4096;
  int __xlx_offset_param_rowPtr_fea1 = 0;
  int __xlx_offset_byte_param_rowPtr_fea1 = 0*4;
  int* __xlx_rowPtr_fea1__input_buffer= new int[__xlx_rowPtr_fea1__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_fea1__tmp_vec.size(); ++i) {
    __xlx_rowPtr_fea1__input_buffer[i] = __xlx_rowPtr_fea1__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_fea2__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_fea2__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_fea2)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_fea2)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_fea2)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_fea2)[j*4+3];
    __xlx_rowPtr_fea2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_fea2 = 4096;
  int __xlx_offset_param_rowPtr_fea2 = 0;
  int __xlx_offset_byte_param_rowPtr_fea2 = 0*4;
  int* __xlx_rowPtr_fea2__input_buffer= new int[__xlx_rowPtr_fea2__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_fea2__tmp_vec.size(); ++i) {
    __xlx_rowPtr_fea2__input_buffer[i] = __xlx_rowPtr_fea2__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_fea3__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_fea3__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_fea3)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_fea3)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_fea3)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_fea3)[j*4+3];
    __xlx_rowPtr_fea3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_fea3 = 4096;
  int __xlx_offset_param_rowPtr_fea3 = 0;
  int __xlx_offset_byte_param_rowPtr_fea3 = 0*4;
  int* __xlx_rowPtr_fea3__input_buffer= new int[__xlx_rowPtr_fea3__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_fea3__tmp_vec.size(); ++i) {
    __xlx_rowPtr_fea3__input_buffer[i] = __xlx_rowPtr_fea3__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_fea4__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_fea4__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_fea4)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_fea4)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_fea4)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_fea4)[j*4+3];
    __xlx_rowPtr_fea4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_fea4 = 4096;
  int __xlx_offset_param_rowPtr_fea4 = 0;
  int __xlx_offset_byte_param_rowPtr_fea4 = 0*4;
  int* __xlx_rowPtr_fea4__input_buffer= new int[__xlx_rowPtr_fea4__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_fea4__tmp_vec.size(); ++i) {
    __xlx_rowPtr_fea4__input_buffer[i] = __xlx_rowPtr_fea4__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_fea1__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_fea1__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_fea1)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_fea1)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_fea1)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_fea1)[j*4+3];
    __xlx_columnIndex_fea1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_fea1 = 128000;
  int __xlx_offset_param_columnIndex_fea1 = 0;
  int __xlx_offset_byte_param_columnIndex_fea1 = 0*4;
  int* __xlx_columnIndex_fea1__input_buffer= new int[__xlx_columnIndex_fea1__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_fea1__tmp_vec.size(); ++i) {
    __xlx_columnIndex_fea1__input_buffer[i] = __xlx_columnIndex_fea1__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_fea2__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_fea2__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_fea2)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_fea2)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_fea2)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_fea2)[j*4+3];
    __xlx_columnIndex_fea2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_fea2 = 128000;
  int __xlx_offset_param_columnIndex_fea2 = 0;
  int __xlx_offset_byte_param_columnIndex_fea2 = 0*4;
  int* __xlx_columnIndex_fea2__input_buffer= new int[__xlx_columnIndex_fea2__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_fea2__tmp_vec.size(); ++i) {
    __xlx_columnIndex_fea2__input_buffer[i] = __xlx_columnIndex_fea2__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_fea3__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_fea3__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_fea3)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_fea3)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_fea3)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_fea3)[j*4+3];
    __xlx_columnIndex_fea3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_fea3 = 128000;
  int __xlx_offset_param_columnIndex_fea3 = 0;
  int __xlx_offset_byte_param_columnIndex_fea3 = 0*4;
  int* __xlx_columnIndex_fea3__input_buffer= new int[__xlx_columnIndex_fea3__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_fea3__tmp_vec.size(); ++i) {
    __xlx_columnIndex_fea3__input_buffer[i] = __xlx_columnIndex_fea3__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_fea4__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_fea4__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_fea4)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_fea4)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_fea4)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_fea4)[j*4+3];
    __xlx_columnIndex_fea4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_fea4 = 128000;
  int __xlx_offset_param_columnIndex_fea4 = 0;
  int __xlx_offset_byte_param_columnIndex_fea4 = 0*4;
  int* __xlx_columnIndex_fea4__input_buffer= new int[__xlx_columnIndex_fea4__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_fea4__tmp_vec.size(); ++i) {
    __xlx_columnIndex_fea4__input_buffer[i] = __xlx_columnIndex_fea4__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_values_fea1__tmp_vec
  vector<sc_bv<16> >__xlx_values_fea1__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_fea1)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_fea1)[j*2+1];
    __xlx_values_fea1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_fea1 = 128000;
  int __xlx_offset_param_values_fea1 = 0;
  int __xlx_offset_byte_param_values_fea1 = 0*2;
  short* __xlx_values_fea1__input_buffer= new short[__xlx_values_fea1__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_fea1__tmp_vec.size(); ++i) {
    __xlx_values_fea1__input_buffer[i] = __xlx_values_fea1__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_values_fea2__tmp_vec
  vector<sc_bv<16> >__xlx_values_fea2__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_fea2)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_fea2)[j*2+1];
    __xlx_values_fea2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_fea2 = 128000;
  int __xlx_offset_param_values_fea2 = 0;
  int __xlx_offset_byte_param_values_fea2 = 0*2;
  short* __xlx_values_fea2__input_buffer= new short[__xlx_values_fea2__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_fea2__tmp_vec.size(); ++i) {
    __xlx_values_fea2__input_buffer[i] = __xlx_values_fea2__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_values_fea3__tmp_vec
  vector<sc_bv<16> >__xlx_values_fea3__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_fea3)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_fea3)[j*2+1];
    __xlx_values_fea3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_fea3 = 128000;
  int __xlx_offset_param_values_fea3 = 0;
  int __xlx_offset_byte_param_values_fea3 = 0*2;
  short* __xlx_values_fea3__input_buffer= new short[__xlx_values_fea3__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_fea3__tmp_vec.size(); ++i) {
    __xlx_values_fea3__input_buffer[i] = __xlx_values_fea3__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_values_fea4__tmp_vec
  vector<sc_bv<16> >__xlx_values_fea4__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_fea4)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_fea4)[j*2+1];
    __xlx_values_fea4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_fea4 = 128000;
  int __xlx_offset_param_values_fea4 = 0;
  int __xlx_offset_byte_param_values_fea4 = 0*2;
  short* __xlx_values_fea4__input_buffer= new short[__xlx_values_fea4__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_fea4__tmp_vec.size(); ++i) {
    __xlx_values_fea4__input_buffer[i] = __xlx_values_fea4__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_adj1__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_adj1__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_adj1)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_adj1)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_adj1)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_adj1)[j*4+3];
    __xlx_rowPtr_adj1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_adj1 = 4096;
  int __xlx_offset_param_rowPtr_adj1 = 0;
  int __xlx_offset_byte_param_rowPtr_adj1 = 0*4;
  int* __xlx_rowPtr_adj1__input_buffer= new int[__xlx_rowPtr_adj1__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_adj1__tmp_vec.size(); ++i) {
    __xlx_rowPtr_adj1__input_buffer[i] = __xlx_rowPtr_adj1__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_adj2__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_adj2__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_adj2)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_adj2)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_adj2)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_adj2)[j*4+3];
    __xlx_rowPtr_adj2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_adj2 = 4096;
  int __xlx_offset_param_rowPtr_adj2 = 0;
  int __xlx_offset_byte_param_rowPtr_adj2 = 0*4;
  int* __xlx_rowPtr_adj2__input_buffer= new int[__xlx_rowPtr_adj2__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_adj2__tmp_vec.size(); ++i) {
    __xlx_rowPtr_adj2__input_buffer[i] = __xlx_rowPtr_adj2__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_adj3__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_adj3__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_adj3)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_adj3)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_adj3)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_adj3)[j*4+3];
    __xlx_rowPtr_adj3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_adj3 = 4096;
  int __xlx_offset_param_rowPtr_adj3 = 0;
  int __xlx_offset_byte_param_rowPtr_adj3 = 0*4;
  int* __xlx_rowPtr_adj3__input_buffer= new int[__xlx_rowPtr_adj3__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_adj3__tmp_vec.size(); ++i) {
    __xlx_rowPtr_adj3__input_buffer[i] = __xlx_rowPtr_adj3__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_rowPtr_adj4__tmp_vec
  vector<sc_bv<32> >__xlx_rowPtr_adj4__tmp_vec;
  for (int j = 0, e = 4096; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_rowPtr_adj4)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_rowPtr_adj4)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_rowPtr_adj4)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_rowPtr_adj4)[j*4+3];
    __xlx_rowPtr_adj4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_rowPtr_adj4 = 4096;
  int __xlx_offset_param_rowPtr_adj4 = 0;
  int __xlx_offset_byte_param_rowPtr_adj4 = 0*4;
  int* __xlx_rowPtr_adj4__input_buffer= new int[__xlx_rowPtr_adj4__tmp_vec.size()];
  for (int i = 0; i < __xlx_rowPtr_adj4__tmp_vec.size(); ++i) {
    __xlx_rowPtr_adj4__input_buffer[i] = __xlx_rowPtr_adj4__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_adj1__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_adj1__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_adj1)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_adj1)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_adj1)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_adj1)[j*4+3];
    __xlx_columnIndex_adj1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_adj1 = 128000;
  int __xlx_offset_param_columnIndex_adj1 = 0;
  int __xlx_offset_byte_param_columnIndex_adj1 = 0*4;
  int* __xlx_columnIndex_adj1__input_buffer= new int[__xlx_columnIndex_adj1__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_adj1__tmp_vec.size(); ++i) {
    __xlx_columnIndex_adj1__input_buffer[i] = __xlx_columnIndex_adj1__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_adj2__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_adj2__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_adj2)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_adj2)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_adj2)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_adj2)[j*4+3];
    __xlx_columnIndex_adj2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_adj2 = 128000;
  int __xlx_offset_param_columnIndex_adj2 = 0;
  int __xlx_offset_byte_param_columnIndex_adj2 = 0*4;
  int* __xlx_columnIndex_adj2__input_buffer= new int[__xlx_columnIndex_adj2__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_adj2__tmp_vec.size(); ++i) {
    __xlx_columnIndex_adj2__input_buffer[i] = __xlx_columnIndex_adj2__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_adj3__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_adj3__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_adj3)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_adj3)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_adj3)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_adj3)[j*4+3];
    __xlx_columnIndex_adj3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_adj3 = 128000;
  int __xlx_offset_param_columnIndex_adj3 = 0;
  int __xlx_offset_byte_param_columnIndex_adj3 = 0*4;
  int* __xlx_columnIndex_adj3__input_buffer= new int[__xlx_columnIndex_adj3__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_adj3__tmp_vec.size(); ++i) {
    __xlx_columnIndex_adj3__input_buffer[i] = __xlx_columnIndex_adj3__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_columnIndex_adj4__tmp_vec
  vector<sc_bv<32> >__xlx_columnIndex_adj4__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<32> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_columnIndex_adj4)[j*4+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_columnIndex_adj4)[j*4+1];
    _xlx_tmp_sc.range(23, 16) = ((char*)__xlx_apatb_param_columnIndex_adj4)[j*4+2];
    _xlx_tmp_sc.range(31, 24) = ((char*)__xlx_apatb_param_columnIndex_adj4)[j*4+3];
    __xlx_columnIndex_adj4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_columnIndex_adj4 = 128000;
  int __xlx_offset_param_columnIndex_adj4 = 0;
  int __xlx_offset_byte_param_columnIndex_adj4 = 0*4;
  int* __xlx_columnIndex_adj4__input_buffer= new int[__xlx_columnIndex_adj4__tmp_vec.size()];
  for (int i = 0; i < __xlx_columnIndex_adj4__tmp_vec.size(); ++i) {
    __xlx_columnIndex_adj4__input_buffer[i] = __xlx_columnIndex_adj4__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_values_adj1__tmp_vec
  vector<sc_bv<16> >__xlx_values_adj1__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_adj1)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_adj1)[j*2+1];
    __xlx_values_adj1__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_adj1 = 128000;
  int __xlx_offset_param_values_adj1 = 0;
  int __xlx_offset_byte_param_values_adj1 = 0*2;
  short* __xlx_values_adj1__input_buffer= new short[__xlx_values_adj1__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_adj1__tmp_vec.size(); ++i) {
    __xlx_values_adj1__input_buffer[i] = __xlx_values_adj1__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_values_adj2__tmp_vec
  vector<sc_bv<16> >__xlx_values_adj2__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_adj2)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_adj2)[j*2+1];
    __xlx_values_adj2__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_adj2 = 128000;
  int __xlx_offset_param_values_adj2 = 0;
  int __xlx_offset_byte_param_values_adj2 = 0*2;
  short* __xlx_values_adj2__input_buffer= new short[__xlx_values_adj2__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_adj2__tmp_vec.size(); ++i) {
    __xlx_values_adj2__input_buffer[i] = __xlx_values_adj2__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_values_adj3__tmp_vec
  vector<sc_bv<16> >__xlx_values_adj3__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_adj3)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_adj3)[j*2+1];
    __xlx_values_adj3__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_adj3 = 128000;
  int __xlx_offset_param_values_adj3 = 0;
  int __xlx_offset_byte_param_values_adj3 = 0*2;
  short* __xlx_values_adj3__input_buffer= new short[__xlx_values_adj3__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_adj3__tmp_vec.size(); ++i) {
    __xlx_values_adj3__input_buffer[i] = __xlx_values_adj3__tmp_vec[i].range(15, 0).to_uint64();
  }
  // Collect __xlx_values_adj4__tmp_vec
  vector<sc_bv<16> >__xlx_values_adj4__tmp_vec;
  for (int j = 0, e = 128000; j != e; ++j) {
    sc_bv<16> _xlx_tmp_sc;
    _xlx_tmp_sc.range(7, 0) = ((char*)__xlx_apatb_param_values_adj4)[j*2+0];
    _xlx_tmp_sc.range(15, 8) = ((char*)__xlx_apatb_param_values_adj4)[j*2+1];
    __xlx_values_adj4__tmp_vec.push_back(_xlx_tmp_sc);
  }
  int __xlx_size_param_values_adj4 = 128000;
  int __xlx_offset_param_values_adj4 = 0;
  int __xlx_offset_byte_param_values_adj4 = 0*2;
  short* __xlx_values_adj4__input_buffer= new short[__xlx_values_adj4__tmp_vec.size()];
  for (int i = 0; i < __xlx_values_adj4__tmp_vec.size(); ++i) {
    __xlx_values_adj4__input_buffer[i] = __xlx_values_adj4__tmp_vec[i].range(15, 0).to_uint64();
  }
  // DUT call
  mmult_top(__xlx_quantized_multiplier__input_buffer, __xlx_shift__input_buffer, __xlx_bias__input_buffer, __xlx_profiling__input_buffer, __xlx_B__input_buffer, __xlx_D1__input_buffer, __xlx_D2__input_buffer, __xlx_D3__input_buffer, __xlx_D4__input_buffer, __xlx_rowPtr_fea1__input_buffer, __xlx_rowPtr_fea2__input_buffer, __xlx_rowPtr_fea3__input_buffer, __xlx_rowPtr_fea4__input_buffer, __xlx_columnIndex_fea1__input_buffer, __xlx_columnIndex_fea2__input_buffer, __xlx_columnIndex_fea3__input_buffer, __xlx_columnIndex_fea4__input_buffer, __xlx_values_fea1__input_buffer, __xlx_values_fea2__input_buffer, __xlx_values_fea3__input_buffer, __xlx_values_fea4__input_buffer, __xlx_rowPtr_adj1__input_buffer, __xlx_rowPtr_adj2__input_buffer, __xlx_rowPtr_adj3__input_buffer, __xlx_rowPtr_adj4__input_buffer, __xlx_columnIndex_adj1__input_buffer, __xlx_columnIndex_adj2__input_buffer, __xlx_columnIndex_adj3__input_buffer, __xlx_columnIndex_adj4__input_buffer, __xlx_values_adj1__input_buffer, __xlx_values_adj2__input_buffer, __xlx_values_adj3__input_buffer, __xlx_values_adj4__input_buffer, __xlx_apatb_param_gemm_mode, __xlx_offset_byte_param_quantized_multiplier, __xlx_offset_byte_param_shift, __xlx_offset_byte_param_bias, *__xlx_apatb_param_bias_count, __xlx_offset_byte_param_profiling, *__xlx_apatb_param_zero_point_lhs, *__xlx_apatb_param_zero_point_rhs, *__xlx_apatb_param_zero_point_dst, *__xlx_apatb_param_clamp_max, *__xlx_apatb_param_clamp_min, __xlx_apatb_param_N_adj, __xlx_apatb_param_M_adj, __xlx_apatb_param_M_fea, __xlx_apatb_param_P_w, __xlx_offset_byte_param_B, __xlx_offset_byte_param_D1, __xlx_offset_byte_param_D2, __xlx_offset_byte_param_D3, __xlx_offset_byte_param_D4, __xlx_apatb_param_array_c_adjust, __xlx_offset_byte_param_rowPtr_fea1, __xlx_offset_byte_param_rowPtr_fea2, __xlx_offset_byte_param_rowPtr_fea3, __xlx_offset_byte_param_rowPtr_fea4, __xlx_offset_byte_param_columnIndex_fea1, __xlx_offset_byte_param_columnIndex_fea2, __xlx_offset_byte_param_columnIndex_fea3, __xlx_offset_byte_param_columnIndex_fea4, __xlx_offset_byte_param_values_fea1, __xlx_offset_byte_param_values_fea2, __xlx_offset_byte_param_values_fea3, __xlx_offset_byte_param_values_fea4, __xlx_offset_byte_param_rowPtr_adj1, __xlx_offset_byte_param_rowPtr_adj2, __xlx_offset_byte_param_rowPtr_adj3, __xlx_offset_byte_param_rowPtr_adj4, __xlx_offset_byte_param_columnIndex_adj1, __xlx_offset_byte_param_columnIndex_adj2, __xlx_offset_byte_param_columnIndex_adj3, __xlx_offset_byte_param_columnIndex_adj4, __xlx_offset_byte_param_values_adj1, __xlx_offset_byte_param_values_adj2, __xlx_offset_byte_param_values_adj3, __xlx_offset_byte_param_values_adj4);
// print __xlx_apatb_param_quantized_multiplier
  sc_bv<32>*__xlx_quantized_multiplier_output_buffer = new sc_bv<32>[__xlx_size_param_quantized_multiplier];
  for (int i = 0; i < __xlx_size_param_quantized_multiplier; ++i) {
    __xlx_quantized_multiplier_output_buffer[i] = __xlx_quantized_multiplier__input_buffer[i+__xlx_offset_param_quantized_multiplier];
  }
  for (int i = 0; i < __xlx_size_param_quantized_multiplier; ++i) {
    ((char*)__xlx_apatb_param_quantized_multiplier)[i*4+0] = __xlx_quantized_multiplier_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_quantized_multiplier)[i*4+1] = __xlx_quantized_multiplier_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_quantized_multiplier)[i*4+2] = __xlx_quantized_multiplier_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_quantized_multiplier)[i*4+3] = __xlx_quantized_multiplier_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_shift
  sc_bv<32>*__xlx_shift_output_buffer = new sc_bv<32>[__xlx_size_param_shift];
  for (int i = 0; i < __xlx_size_param_shift; ++i) {
    __xlx_shift_output_buffer[i] = __xlx_shift__input_buffer[i+__xlx_offset_param_shift];
  }
  for (int i = 0; i < __xlx_size_param_shift; ++i) {
    ((char*)__xlx_apatb_param_shift)[i*4+0] = __xlx_shift_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_shift)[i*4+1] = __xlx_shift_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_shift)[i*4+2] = __xlx_shift_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_shift)[i*4+3] = __xlx_shift_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_bias
  sc_bv<32>*__xlx_bias_output_buffer = new sc_bv<32>[__xlx_size_param_bias];
  for (int i = 0; i < __xlx_size_param_bias; ++i) {
    __xlx_bias_output_buffer[i] = __xlx_bias__input_buffer[i+__xlx_offset_param_bias];
  }
  for (int i = 0; i < __xlx_size_param_bias; ++i) {
    ((char*)__xlx_apatb_param_bias)[i*4+0] = __xlx_bias_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_bias)[i*4+1] = __xlx_bias_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_bias)[i*4+2] = __xlx_bias_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_bias)[i*4+3] = __xlx_bias_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_profiling
  sc_bv<64>*__xlx_profiling_output_buffer = new sc_bv<64>[__xlx_size_param_profiling];
  for (int i = 0; i < __xlx_size_param_profiling; ++i) {
    __xlx_profiling_output_buffer[i] = __xlx_profiling__input_buffer[i+__xlx_offset_param_profiling];
  }
  for (int i = 0; i < __xlx_size_param_profiling; ++i) {
    ((char*)__xlx_apatb_param_profiling)[i*8+0] = __xlx_profiling_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+1] = __xlx_profiling_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+2] = __xlx_profiling_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+3] = __xlx_profiling_output_buffer[i].range(31, 24).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+4] = __xlx_profiling_output_buffer[i].range(39, 32).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+5] = __xlx_profiling_output_buffer[i].range(47, 40).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+6] = __xlx_profiling_output_buffer[i].range(55, 48).to_uint();
    ((char*)__xlx_apatb_param_profiling)[i*8+7] = __xlx_profiling_output_buffer[i].range(63, 56).to_uint();
  }
// print __xlx_apatb_param_B
  sc_bv<16>*__xlx_B_output_buffer = new sc_bv<16>[__xlx_size_param_B];
  for (int i = 0; i < __xlx_size_param_B; ++i) {
    __xlx_B_output_buffer[i] = __xlx_B__input_buffer[i+__xlx_offset_param_B];
  }
  for (int i = 0; i < __xlx_size_param_B; ++i) {
    ((char*)__xlx_apatb_param_B)[i*2+0] = __xlx_B_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_B)[i*2+1] = __xlx_B_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_D1
  sc_bv<16>*__xlx_D1_output_buffer = new sc_bv<16>[__xlx_size_param_D1];
  for (int i = 0; i < __xlx_size_param_D1; ++i) {
    __xlx_D1_output_buffer[i] = __xlx_D1__input_buffer[i+__xlx_offset_param_D1];
  }
  for (int i = 0; i < __xlx_size_param_D1; ++i) {
    ((char*)__xlx_apatb_param_D1)[i*2+0] = __xlx_D1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_D1)[i*2+1] = __xlx_D1_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_D2
  sc_bv<16>*__xlx_D2_output_buffer = new sc_bv<16>[__xlx_size_param_D2];
  for (int i = 0; i < __xlx_size_param_D2; ++i) {
    __xlx_D2_output_buffer[i] = __xlx_D2__input_buffer[i+__xlx_offset_param_D2];
  }
  for (int i = 0; i < __xlx_size_param_D2; ++i) {
    ((char*)__xlx_apatb_param_D2)[i*2+0] = __xlx_D2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_D2)[i*2+1] = __xlx_D2_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_D3
  sc_bv<16>*__xlx_D3_output_buffer = new sc_bv<16>[__xlx_size_param_D3];
  for (int i = 0; i < __xlx_size_param_D3; ++i) {
    __xlx_D3_output_buffer[i] = __xlx_D3__input_buffer[i+__xlx_offset_param_D3];
  }
  for (int i = 0; i < __xlx_size_param_D3; ++i) {
    ((char*)__xlx_apatb_param_D3)[i*2+0] = __xlx_D3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_D3)[i*2+1] = __xlx_D3_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_D4
  sc_bv<16>*__xlx_D4_output_buffer = new sc_bv<16>[__xlx_size_param_D4];
  for (int i = 0; i < __xlx_size_param_D4; ++i) {
    __xlx_D4_output_buffer[i] = __xlx_D4__input_buffer[i+__xlx_offset_param_D4];
  }
  for (int i = 0; i < __xlx_size_param_D4; ++i) {
    ((char*)__xlx_apatb_param_D4)[i*2+0] = __xlx_D4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_D4)[i*2+1] = __xlx_D4_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_rowPtr_fea1
  sc_bv<32>*__xlx_rowPtr_fea1_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_fea1];
  for (int i = 0; i < __xlx_size_param_rowPtr_fea1; ++i) {
    __xlx_rowPtr_fea1_output_buffer[i] = __xlx_rowPtr_fea1__input_buffer[i+__xlx_offset_param_rowPtr_fea1];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_fea1; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_fea1)[i*4+0] = __xlx_rowPtr_fea1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea1)[i*4+1] = __xlx_rowPtr_fea1_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea1)[i*4+2] = __xlx_rowPtr_fea1_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea1)[i*4+3] = __xlx_rowPtr_fea1_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_rowPtr_fea2
  sc_bv<32>*__xlx_rowPtr_fea2_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_fea2];
  for (int i = 0; i < __xlx_size_param_rowPtr_fea2; ++i) {
    __xlx_rowPtr_fea2_output_buffer[i] = __xlx_rowPtr_fea2__input_buffer[i+__xlx_offset_param_rowPtr_fea2];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_fea2; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_fea2)[i*4+0] = __xlx_rowPtr_fea2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea2)[i*4+1] = __xlx_rowPtr_fea2_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea2)[i*4+2] = __xlx_rowPtr_fea2_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea2)[i*4+3] = __xlx_rowPtr_fea2_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_rowPtr_fea3
  sc_bv<32>*__xlx_rowPtr_fea3_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_fea3];
  for (int i = 0; i < __xlx_size_param_rowPtr_fea3; ++i) {
    __xlx_rowPtr_fea3_output_buffer[i] = __xlx_rowPtr_fea3__input_buffer[i+__xlx_offset_param_rowPtr_fea3];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_fea3; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_fea3)[i*4+0] = __xlx_rowPtr_fea3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea3)[i*4+1] = __xlx_rowPtr_fea3_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea3)[i*4+2] = __xlx_rowPtr_fea3_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea3)[i*4+3] = __xlx_rowPtr_fea3_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_rowPtr_fea4
  sc_bv<32>*__xlx_rowPtr_fea4_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_fea4];
  for (int i = 0; i < __xlx_size_param_rowPtr_fea4; ++i) {
    __xlx_rowPtr_fea4_output_buffer[i] = __xlx_rowPtr_fea4__input_buffer[i+__xlx_offset_param_rowPtr_fea4];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_fea4; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_fea4)[i*4+0] = __xlx_rowPtr_fea4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea4)[i*4+1] = __xlx_rowPtr_fea4_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea4)[i*4+2] = __xlx_rowPtr_fea4_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_fea4)[i*4+3] = __xlx_rowPtr_fea4_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_fea1
  sc_bv<32>*__xlx_columnIndex_fea1_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_fea1];
  for (int i = 0; i < __xlx_size_param_columnIndex_fea1; ++i) {
    __xlx_columnIndex_fea1_output_buffer[i] = __xlx_columnIndex_fea1__input_buffer[i+__xlx_offset_param_columnIndex_fea1];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_fea1; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_fea1)[i*4+0] = __xlx_columnIndex_fea1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea1)[i*4+1] = __xlx_columnIndex_fea1_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea1)[i*4+2] = __xlx_columnIndex_fea1_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea1)[i*4+3] = __xlx_columnIndex_fea1_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_fea2
  sc_bv<32>*__xlx_columnIndex_fea2_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_fea2];
  for (int i = 0; i < __xlx_size_param_columnIndex_fea2; ++i) {
    __xlx_columnIndex_fea2_output_buffer[i] = __xlx_columnIndex_fea2__input_buffer[i+__xlx_offset_param_columnIndex_fea2];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_fea2; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_fea2)[i*4+0] = __xlx_columnIndex_fea2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea2)[i*4+1] = __xlx_columnIndex_fea2_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea2)[i*4+2] = __xlx_columnIndex_fea2_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea2)[i*4+3] = __xlx_columnIndex_fea2_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_fea3
  sc_bv<32>*__xlx_columnIndex_fea3_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_fea3];
  for (int i = 0; i < __xlx_size_param_columnIndex_fea3; ++i) {
    __xlx_columnIndex_fea3_output_buffer[i] = __xlx_columnIndex_fea3__input_buffer[i+__xlx_offset_param_columnIndex_fea3];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_fea3; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_fea3)[i*4+0] = __xlx_columnIndex_fea3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea3)[i*4+1] = __xlx_columnIndex_fea3_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea3)[i*4+2] = __xlx_columnIndex_fea3_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea3)[i*4+3] = __xlx_columnIndex_fea3_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_fea4
  sc_bv<32>*__xlx_columnIndex_fea4_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_fea4];
  for (int i = 0; i < __xlx_size_param_columnIndex_fea4; ++i) {
    __xlx_columnIndex_fea4_output_buffer[i] = __xlx_columnIndex_fea4__input_buffer[i+__xlx_offset_param_columnIndex_fea4];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_fea4; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_fea4)[i*4+0] = __xlx_columnIndex_fea4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea4)[i*4+1] = __xlx_columnIndex_fea4_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea4)[i*4+2] = __xlx_columnIndex_fea4_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_fea4)[i*4+3] = __xlx_columnIndex_fea4_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_values_fea1
  sc_bv<16>*__xlx_values_fea1_output_buffer = new sc_bv<16>[__xlx_size_param_values_fea1];
  for (int i = 0; i < __xlx_size_param_values_fea1; ++i) {
    __xlx_values_fea1_output_buffer[i] = __xlx_values_fea1__input_buffer[i+__xlx_offset_param_values_fea1];
  }
  for (int i = 0; i < __xlx_size_param_values_fea1; ++i) {
    ((char*)__xlx_apatb_param_values_fea1)[i*2+0] = __xlx_values_fea1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_fea1)[i*2+1] = __xlx_values_fea1_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_values_fea2
  sc_bv<16>*__xlx_values_fea2_output_buffer = new sc_bv<16>[__xlx_size_param_values_fea2];
  for (int i = 0; i < __xlx_size_param_values_fea2; ++i) {
    __xlx_values_fea2_output_buffer[i] = __xlx_values_fea2__input_buffer[i+__xlx_offset_param_values_fea2];
  }
  for (int i = 0; i < __xlx_size_param_values_fea2; ++i) {
    ((char*)__xlx_apatb_param_values_fea2)[i*2+0] = __xlx_values_fea2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_fea2)[i*2+1] = __xlx_values_fea2_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_values_fea3
  sc_bv<16>*__xlx_values_fea3_output_buffer = new sc_bv<16>[__xlx_size_param_values_fea3];
  for (int i = 0; i < __xlx_size_param_values_fea3; ++i) {
    __xlx_values_fea3_output_buffer[i] = __xlx_values_fea3__input_buffer[i+__xlx_offset_param_values_fea3];
  }
  for (int i = 0; i < __xlx_size_param_values_fea3; ++i) {
    ((char*)__xlx_apatb_param_values_fea3)[i*2+0] = __xlx_values_fea3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_fea3)[i*2+1] = __xlx_values_fea3_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_values_fea4
  sc_bv<16>*__xlx_values_fea4_output_buffer = new sc_bv<16>[__xlx_size_param_values_fea4];
  for (int i = 0; i < __xlx_size_param_values_fea4; ++i) {
    __xlx_values_fea4_output_buffer[i] = __xlx_values_fea4__input_buffer[i+__xlx_offset_param_values_fea4];
  }
  for (int i = 0; i < __xlx_size_param_values_fea4; ++i) {
    ((char*)__xlx_apatb_param_values_fea4)[i*2+0] = __xlx_values_fea4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_fea4)[i*2+1] = __xlx_values_fea4_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_rowPtr_adj1
  sc_bv<32>*__xlx_rowPtr_adj1_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_adj1];
  for (int i = 0; i < __xlx_size_param_rowPtr_adj1; ++i) {
    __xlx_rowPtr_adj1_output_buffer[i] = __xlx_rowPtr_adj1__input_buffer[i+__xlx_offset_param_rowPtr_adj1];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_adj1; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_adj1)[i*4+0] = __xlx_rowPtr_adj1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj1)[i*4+1] = __xlx_rowPtr_adj1_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj1)[i*4+2] = __xlx_rowPtr_adj1_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj1)[i*4+3] = __xlx_rowPtr_adj1_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_rowPtr_adj2
  sc_bv<32>*__xlx_rowPtr_adj2_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_adj2];
  for (int i = 0; i < __xlx_size_param_rowPtr_adj2; ++i) {
    __xlx_rowPtr_adj2_output_buffer[i] = __xlx_rowPtr_adj2__input_buffer[i+__xlx_offset_param_rowPtr_adj2];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_adj2; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_adj2)[i*4+0] = __xlx_rowPtr_adj2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj2)[i*4+1] = __xlx_rowPtr_adj2_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj2)[i*4+2] = __xlx_rowPtr_adj2_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj2)[i*4+3] = __xlx_rowPtr_adj2_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_rowPtr_adj3
  sc_bv<32>*__xlx_rowPtr_adj3_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_adj3];
  for (int i = 0; i < __xlx_size_param_rowPtr_adj3; ++i) {
    __xlx_rowPtr_adj3_output_buffer[i] = __xlx_rowPtr_adj3__input_buffer[i+__xlx_offset_param_rowPtr_adj3];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_adj3; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_adj3)[i*4+0] = __xlx_rowPtr_adj3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj3)[i*4+1] = __xlx_rowPtr_adj3_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj3)[i*4+2] = __xlx_rowPtr_adj3_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj3)[i*4+3] = __xlx_rowPtr_adj3_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_rowPtr_adj4
  sc_bv<32>*__xlx_rowPtr_adj4_output_buffer = new sc_bv<32>[__xlx_size_param_rowPtr_adj4];
  for (int i = 0; i < __xlx_size_param_rowPtr_adj4; ++i) {
    __xlx_rowPtr_adj4_output_buffer[i] = __xlx_rowPtr_adj4__input_buffer[i+__xlx_offset_param_rowPtr_adj4];
  }
  for (int i = 0; i < __xlx_size_param_rowPtr_adj4; ++i) {
    ((char*)__xlx_apatb_param_rowPtr_adj4)[i*4+0] = __xlx_rowPtr_adj4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj4)[i*4+1] = __xlx_rowPtr_adj4_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj4)[i*4+2] = __xlx_rowPtr_adj4_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_rowPtr_adj4)[i*4+3] = __xlx_rowPtr_adj4_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_adj1
  sc_bv<32>*__xlx_columnIndex_adj1_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_adj1];
  for (int i = 0; i < __xlx_size_param_columnIndex_adj1; ++i) {
    __xlx_columnIndex_adj1_output_buffer[i] = __xlx_columnIndex_adj1__input_buffer[i+__xlx_offset_param_columnIndex_adj1];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_adj1; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_adj1)[i*4+0] = __xlx_columnIndex_adj1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj1)[i*4+1] = __xlx_columnIndex_adj1_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj1)[i*4+2] = __xlx_columnIndex_adj1_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj1)[i*4+3] = __xlx_columnIndex_adj1_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_adj2
  sc_bv<32>*__xlx_columnIndex_adj2_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_adj2];
  for (int i = 0; i < __xlx_size_param_columnIndex_adj2; ++i) {
    __xlx_columnIndex_adj2_output_buffer[i] = __xlx_columnIndex_adj2__input_buffer[i+__xlx_offset_param_columnIndex_adj2];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_adj2; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_adj2)[i*4+0] = __xlx_columnIndex_adj2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj2)[i*4+1] = __xlx_columnIndex_adj2_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj2)[i*4+2] = __xlx_columnIndex_adj2_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj2)[i*4+3] = __xlx_columnIndex_adj2_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_adj3
  sc_bv<32>*__xlx_columnIndex_adj3_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_adj3];
  for (int i = 0; i < __xlx_size_param_columnIndex_adj3; ++i) {
    __xlx_columnIndex_adj3_output_buffer[i] = __xlx_columnIndex_adj3__input_buffer[i+__xlx_offset_param_columnIndex_adj3];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_adj3; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_adj3)[i*4+0] = __xlx_columnIndex_adj3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj3)[i*4+1] = __xlx_columnIndex_adj3_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj3)[i*4+2] = __xlx_columnIndex_adj3_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj3)[i*4+3] = __xlx_columnIndex_adj3_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_columnIndex_adj4
  sc_bv<32>*__xlx_columnIndex_adj4_output_buffer = new sc_bv<32>[__xlx_size_param_columnIndex_adj4];
  for (int i = 0; i < __xlx_size_param_columnIndex_adj4; ++i) {
    __xlx_columnIndex_adj4_output_buffer[i] = __xlx_columnIndex_adj4__input_buffer[i+__xlx_offset_param_columnIndex_adj4];
  }
  for (int i = 0; i < __xlx_size_param_columnIndex_adj4; ++i) {
    ((char*)__xlx_apatb_param_columnIndex_adj4)[i*4+0] = __xlx_columnIndex_adj4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj4)[i*4+1] = __xlx_columnIndex_adj4_output_buffer[i].range(15, 8).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj4)[i*4+2] = __xlx_columnIndex_adj4_output_buffer[i].range(23, 16).to_uint();
    ((char*)__xlx_apatb_param_columnIndex_adj4)[i*4+3] = __xlx_columnIndex_adj4_output_buffer[i].range(31, 24).to_uint();
  }
// print __xlx_apatb_param_values_adj1
  sc_bv<16>*__xlx_values_adj1_output_buffer = new sc_bv<16>[__xlx_size_param_values_adj1];
  for (int i = 0; i < __xlx_size_param_values_adj1; ++i) {
    __xlx_values_adj1_output_buffer[i] = __xlx_values_adj1__input_buffer[i+__xlx_offset_param_values_adj1];
  }
  for (int i = 0; i < __xlx_size_param_values_adj1; ++i) {
    ((char*)__xlx_apatb_param_values_adj1)[i*2+0] = __xlx_values_adj1_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_adj1)[i*2+1] = __xlx_values_adj1_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_values_adj2
  sc_bv<16>*__xlx_values_adj2_output_buffer = new sc_bv<16>[__xlx_size_param_values_adj2];
  for (int i = 0; i < __xlx_size_param_values_adj2; ++i) {
    __xlx_values_adj2_output_buffer[i] = __xlx_values_adj2__input_buffer[i+__xlx_offset_param_values_adj2];
  }
  for (int i = 0; i < __xlx_size_param_values_adj2; ++i) {
    ((char*)__xlx_apatb_param_values_adj2)[i*2+0] = __xlx_values_adj2_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_adj2)[i*2+1] = __xlx_values_adj2_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_values_adj3
  sc_bv<16>*__xlx_values_adj3_output_buffer = new sc_bv<16>[__xlx_size_param_values_adj3];
  for (int i = 0; i < __xlx_size_param_values_adj3; ++i) {
    __xlx_values_adj3_output_buffer[i] = __xlx_values_adj3__input_buffer[i+__xlx_offset_param_values_adj3];
  }
  for (int i = 0; i < __xlx_size_param_values_adj3; ++i) {
    ((char*)__xlx_apatb_param_values_adj3)[i*2+0] = __xlx_values_adj3_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_adj3)[i*2+1] = __xlx_values_adj3_output_buffer[i].range(15, 8).to_uint();
  }
// print __xlx_apatb_param_values_adj4
  sc_bv<16>*__xlx_values_adj4_output_buffer = new sc_bv<16>[__xlx_size_param_values_adj4];
  for (int i = 0; i < __xlx_size_param_values_adj4; ++i) {
    __xlx_values_adj4_output_buffer[i] = __xlx_values_adj4__input_buffer[i+__xlx_offset_param_values_adj4];
  }
  for (int i = 0; i < __xlx_size_param_values_adj4; ++i) {
    ((char*)__xlx_apatb_param_values_adj4)[i*2+0] = __xlx_values_adj4_output_buffer[i].range(7, 0).to_uint();
    ((char*)__xlx_apatb_param_values_adj4)[i*2+1] = __xlx_values_adj4_output_buffer[i].range(15, 8).to_uint();
  }
}
