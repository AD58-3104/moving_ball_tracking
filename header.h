#ifndef HEADER_H 
#define HEADER_H 

#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <random>
#include <tuple>



class multivariate_normal{
    public:
    multivariate_normal();
    multivariate_normal(const std::map<std::string,double> &);
    std::vector<double> rvs();
    private:
    //std::map<std::string,double> Motion_Noise;
    std::random_device Seed_gen;
    std::default_random_engine Engine;
    std::vector<std::normal_distribution<>> Dist;
};




class MotionModel {
    
    public:
    MotionModel();
    MotionModel(const double,const double);
    MotionModel(const double,const double,const std:: map<std::string,double> &);
    MotionModel(const double,const double,const std:: map<std::string,double> &,const double,const double);

    ~MotionModel();
    std::tuple<double,double> update(double,double,bool);
    std::tuple<double,double> stuck(double,double);

    private:
    const double initial_pose;
    const double time_interval;
    double stuck_time;
    double escape_time;
    
    //標準偏差
    std:: map<std::string,double> motion_noise;

    std::exponential_distribution<> stuck_pdf;
    std::exponential_distribution<> escape_pdf;

    std::random_device seed_gen;
    std::default_random_engine engine;

    
    multivariate_normal motion_noise_rate_pdf;

    double grass_friction_noise;
    double prev_val_x;
    double prev_val_y;
    bool is_stuck;
    double time_until_stuck;
    double time_until_escape;
};

#endif //HEADER_H
