#include "header.h"
multivariate_normal::multivariate_normal(){
    
}
//引数にmapのオブジェクトを渡してください
multivariate_normal::multivariate_normal(const std::map<std::string,double> &data){
    std::random_device Seed_gen;
    std::default_random_engine Engine(Seed_gen());
    std::vector<std::normal_distribution<>> Dist;
    for(auto &object :data){
        //平均と標準偏差をいれるのか？？
        std::normal_distribution<> tmp(object.second*object.second);
        Dist.push_back(tmp);
    }
}
//xx xy yx yyの順に乱数を格納したvectorオブジェクトを返す
std::vector<double> multivariate_normal::rvs(){
    std::vector<double> return_value;
    for(auto &ob: Dist){
        return_value.push_back(ob(Engine));
    }
    return return_value;
}

class ObservationModel
{
    private:
//static_observation_noise[0]="xx", static_observation_noise[1]="xy", dynamic_observation_noise[0]="yx", dynamic_observation_noise[1]="yy"  
        std::map<char, double> m;
        m[xx]=0.15;
        m[xy]=0.15;
        m[yx]=0.01;
        m[yy]=0.01;

        std::tuple<double, double> environment_noise(0.10,0.10);
        std::tuple<double, double> observation_bias(1.8,0.4);

        velo_observation_bias_rate_pdf =  std::normal_distribution<>(get<0>(observation_bias),get<1>(observation_bias));

        std::tuple<double,double>  ObservationModel::update(ball_lc())
        {
            observation_noise_rate_pdf = multivariate_normal(m);       
            noise_x = noise[0] + noise[2];
            noise_y = noise[1] + noise[3];

            get<0>(ball_lc) += noise_x*1000;
            get<1>(ball_lc) += noise_y*1000;

            return std::make_tuple(get<0>(ball_lc),get<1>(ball_lc));
        }
}
