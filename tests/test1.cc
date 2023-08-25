
#include "ntuples/ntuples.hh"
  
  

  ax_new_axis(subdetector, int());

  

  

//  nt_new_axis_t(sector , int() );
  //namespace __nt{       __nt_new_axis_core( sector); static const  auto s = nt::ax_name_container<ztsector>{} = int();       } 
  //using sector = decltype( __nt::s );
  nt_new_axis_t(sector , int{});
  nt_new_axis_c(sector1, std::string());

  using ex1 = decltype( sector1 );

  nt_new_name(axis);
  nt_new_name_t(plane);

int main(int argv, char** argc) {

  auto klm_nameing  = nt::fill_dataframe(2 , [](int i) {

    return nt::ntuple{
      subdetector = i ,
      sector{123},
      sector1 = std::string("asd"),
      axis = "4655",
      plane() = 1231
    };
    }
    );

 
  std::cout << axis(klm_nameing[0]) << std::endl;
  auto x = ax_maker(ex1) = 456;





  auto x123123 = ax_maker(ex6) = std::string("asd");
  //x123123 = "asdaswqe";

  auto x123 = ax_maker(ex6) = 21321;


  auto df   = nt::dataframe<decltype(x123)>();
  df.ex6.push_back(123);

  auto x12 = x123.get(df);


  auto nt3 = nt::ntuple(x123,
    ax_maker(ex7) = 64
  );
  std::cout << nt3 << std::endl;
  std::cout << nt3[df.ex6] << std::endl;
  auto nt4 = nt::ntuple<decltype(nt3.ex7)&>(
    nt3.ex7
  );
  decltype(nt3.ex6)& ref = nt3.ex6;

  nt4.ex7 = 87;
  std::cout << nt4 << std::endl;
  std::cout << nt3[nt3.ex6] << std::endl;
  nt3[nt3.ex6] = 464;
  std::cout << nt3[nt3.ex6] << std::endl;
  [](const auto& e) {
    std::cout << e << std::endl;
  }(nt3);


  auto df1 = nt::fill_dataframe(100, [](int i) {
    return nt::ntuple(
      ax_maker(ex1) = i,
      ax_maker(ex2) = i * i,
      ax_maker(ex3) = i * i * i
    );
    });


  std::cout << "\n\n" << std::endl;
  std::cout << df1 << std::endl;
  std::cout << "\n\n" << std::endl;
  std::cout << df1[5] << std::endl;
  df1[5].ex1 = 123213;
  df1.ex2[5] = 321;
  std::cout << df1[df1.ex2][5] << std::endl;
  

  std::cout << df1[4] << std::endl;
  df1.ex2[4] = 5464;
  std::cout << df1[4] << std::endl;
  df1[2].ex1 = 12321;
  std::cout << df1[2] << std::endl;

  return 0;
}
