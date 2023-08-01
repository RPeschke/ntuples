
#include "ntuples/ntuples.hh"



int main(int argv, char** argc) {

  auto x123 = ax_maker(ex6) = 654;


  auto df = dataframe<decltype(x123)>();
  df.ex6.push_back(123);

  auto x12 = decltype(x123)::struct_maker::get(df);


  auto nt3 = ntuple(x123,
    ax_maker(ex7) = 64
  );
  std::cout << nt3 << std::endl;
  std::cout << nt3[df.ex6] << std::endl;
  auto nt4 = ntuple<decltype(nt3.ex7)&>(
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


  auto df1 = fill_dataframe(100, [](int i) {
    return ntuple(
      ax_maker(ex1) = i,
      ax_maker(ex2) = i * i
    );
    });

  std::cout << df1 << std::endl;
  std::cout << df1[5] << std::endl;
  df1[5].ex1 = 123213;
  df1.ex2[5] = 321;
  std::cout << df1[df1.ex2][5] << std::endl;


  return 1;
}
