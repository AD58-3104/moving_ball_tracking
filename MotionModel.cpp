/*分からない事
１継承されているobjectくんはどこにある
２変数の型どれにするのかしらんから聞く*/

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

    for(auto &ob: Dist){


//一番ありそうなやつ取り合えずこれだけ
MotionModel::MotionModel(const double InitPose,const double TmIntrvl)
    : initial_pose(InitPose), time_interval(TmIntrvl), stuck_time(1e-10), escape_time(1e10), grass_friction_noise(0.75)
{
    motion_noise["xx"] = 0.20;
    motion_noise["xy"] = 0.15;
    motion_noise["yx"] = 0.10;
    motion_noise["yy"] = 0.09;
    multivariate_normal motion_noise_rate_pdf(motion_noise);
    prev_val_x = 0.0;
    prev_val_y = 0.0;
    is_stuck = false;


    std::exponential_distribution<> stuck_pdf(1.0 / stuck_time);
    std::exponential_distribution<> escape_pdf(1.0 / escape_time);

    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    time_until_stuck = stuck_pdf(engine);
    time_until_escape = escape_pdf(engine);
}

MotionModel::MotionModel(const double InitPose,const double TmIntrvl,const std:: map<std::string,double> &mp)
    : initial_pose(InitPose), time_interval(TmIntrvl),motion_noise(mp), stuck_time(1e-10), escape_time(1e10), grass_friction_noise(0.75)
{
    
    multivariate_normal motion_noise_rate_pdf(motion_noise);
    prev_val_x = 0.0;
    prev_val_y = 0.0;
    is_stuck = false;


    std::exponential_distribution<> stuck_pdf(1.0 / stuck_time);
    std::exponential_distribution<> escape_pdf(1.0 / escape_time);

    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    time_until_stuck = stuck_pdf(engine);
    time_until_escape = escape_pdf(engine);
}

MotionModel::MotionModel(const double InitPose,const double TmIntrvl,const std:: map<std::string,double> &mp,const double StkTm,const double EspTm)
    : initial_pose(InitPose), time_interval(TmIntrvl),motion_noise(mp), stuck_time(StkTm), escape_time(EspTm), grass_friction_noise(0.75)
{
    
    multivariate_normal motion_noise_rate_pdf(motion_noise);
    prev_val_x = 0.0;
    prev_val_y = 0.0;
    is_stuck = false;


    std::exponential_distribution<> stuck_pdf(1.0 / stuck_time);
    std::exponential_distribution<> escape_pdf(1.0 / escape_time);

    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    time_until_stuck = stuck_pdf(engine);
    time_until_escape = escape_pdf(engine);
}

MotionModel::~MotionModel() {}

std::tuple<double, double> MotionModel::update(double vel_x, double vel_y, bool use_past_data)
{
    typedef struct
    {
        double x;
        double y;
    } Return_vel;

    Return_vel return_vel;
    if (use_past_data)
    {
        return_vel.x = prev_val_x;
        return_vel.y = prev_val_y;
    }
    else
    {
        return_vel.x = vel_x;
        return_vel.y = vel_y;
    }

    if (std::abs(return_vel.x) > 0.04 || std::abs(return_vel.y) > 0.04)
    {
        std::tie(return_vel.x,return_vel.y) = MotionModel::stuck(return_vel.x,return_vel.y);
        prev_val_x = return_vel.x * grass_friction_noise;
        prev_val_y = return_vel.y * grass_friction_noise;
    }
    else{
        return_vel.x = 0.0;
        return_vel.y = 0.0;
    }

    return std::make_tuple(return_vel.x,return_vel.y);
};

std::tuple<double,double> MotionModel::stuck(double vel_x,double vel_y){
    if(is_stuck){
        time_until_escape -= time_interval;
        if(time_until_escape <= 0.0){
            time_until_escape += escape_pdf(engine);
            is_stuck = true;
        }
    }
    else{
        time_until_stuck -= time_interval;
        if(time_until_stuck <= 0.0){
            time_until_stuck += stuck_pdf(engine);
            is_stuck = true;
        }
    }
    return std::make_tuple(vel_x * static_cast<int>(is_stuck),vel_y * static_cast<int>(is_stuck));
}

int main(){
    return 0;
}
