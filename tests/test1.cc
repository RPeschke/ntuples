
#include "ntuples/ntuples.hh"
  
  

  ax_new_axis(subdetector, int());

  namespace KLM_Helpers {
    ax_new_axis(subdetector_name, std::string());

  }

  ax_new_axis_t(sector , int() );

int main(int argv, char** argc) {

  auto klm_nameing  = nt::fill_dataframe(2 , [](int i) {

    return nt::ntuple{
      subdetector = i ,
      KLM_Helpers::subdetector_name = [&]{  if (i == 1) return  "EKLM" ; return  "BKLM";}(),
      sector{1}
    };
    }
    );


  std::cout << klm_nameing << std::endl;



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
