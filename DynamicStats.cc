#include "DynamicStats.h"

namespace Gmr
{

    Stage::Stage ()
    {
        tau_ = 0.0;
        nu_ = 0.0;
        distance_ = 0.0;
        dx_ = 0.0;
        dy_ = 0.0;
        dz_ = 0.0;
    }

    void   Stage::stage (Stat stat, double x)
    {
        switch (stat)
        {
            case Stat::tau : 
                tau_ += x;
                break;

            case Stat::nu : 
                nu_ += x;
                break;

            case Stat::distance : 
                distance_ += x;
                break;

            case Stat::dx : 
                dx_ += x;
                break;

            case Stat::dy : 
                dy_ += x;
                break;

            case Stat::dz : 
                dz_ += x;
                break;

        }
    }

    double Stage::clear (Stat stat)
    {
        switch (stat)
        {
            case Stat::tau : 
            {
                double answer = tau_;
                tau_ = 0.0;
                return answer;
            }

            case Stat::nu : 
            {
                double answer = nu_;
                nu_ = 0.0;
                return answer;
            }

            case Stat::distance : 
            {
                double answer = distance_;
                distance_ = 0.0;
                return answer;
            }

            case Stat::dx : 
            {
                double answer = dx_;
                dx_ = 0.0;
                return answer;
            }

            case Stat::dy : 
            {
                double answer = dy_;
                dy_ = 0.0;
                return answer;
            }

            case Stat::dz : 
            {
                double answer = dz_;
                dz_ = 0.0;
                return answer;
            }

        }
    }


    DynamicStats::DynamicStats() {}
    
    DynamicStats::DynamicStats(double h, double max, double min)
    {
        for (double i = min; i < max; i += h)
            walls_[i] = 0;
    }

    DynamicStats::DynamicStats(int n, double max, double min)
    {
        double h = (max - min) / n;
        for (double i = min; i < max; i += h)
            walls_[i] = 0;
    }

    DynamicStats::~DynamicStats() {}

    void DynamicStats::record (Particle* p)
    {
        stage_map::iterator it = stages_.find(p);
        
        if (it == stages_.end())
        {
            stages_[p] = Stage();
            it = stages_.find(p);
        }

        tau_.accumulate((it -> second).clear(Stat::tau));
        (it -> second).stage(Stat::nu, 1.0);
        distance_.accumulate((it -> second).clear(Stat::distance));
        dx_.accumulate((it -> second).clear(Stat::dx));
        dy_.accumulate((it -> second).clear(Stat::dy));
        dz_.accumulate((it -> second).clear(Stat::dz));
    }

    void DynamicStats::record (Particle* p, const darray& a, const darray& b)
    {
        stage_map::iterator it = stages_.find(p);
        
        if (it == stages_.end())
        {
            stages_[p] = Stage();
            it = stages_.find(p);
        }

        (it -> second).stage(Stat::tau, 1.0);
        nu_.accumulate((it -> second).clear(Stat::nu));
        (it -> second).stage(Stat::distance, distance(a, b));
        (it -> second).stage(Stat::dx, b[0] - a[0]);
        (it -> second).stage(Stat::dy, b[1] - a[1]);
        (it -> second).stage(Stat::dz, b[2] - a[2]);

        for(auto&& wall : walls_)
        {
            if (a[0] < wall.first && b[0] >= wall.first)
                wall.second += (p -> getCharge()) < 0 ? - 1 : 1;

            if (a[0] > wall.first && b[0] <= wall.first)
                wall.second -= (p -> getCharge()) < 0 ? - 1 : 1;
        }

    }


    double DynamicStats::mean (Stat stat)
    {
        switch (stat)
        {
            case Stat::tau : 
                return tau_.mean();

            case Stat::nu : 
                return nu_.mean();

            case Stat::distance : 
                return distance_.mean();

            case Stat::dx : 
                return dx_.mean();

            case Stat::dy : 
                return dy_.mean();

            case Stat::dz : 
                return dz_.mean();

            case Stat::wall :
            {
                if (walls_.size() == 0) return 0;
                Accumulator wall_accumulator;
                for (auto&& wall : walls_)
                    wall_accumulator.accumulate(wall.second);
                return wall_accumulator.mean();
            }

        }
    }

    double DynamicStats::stddev (Stat stat)
    {
        switch (stat)
        {
            case Stat::tau : 
                return tau_.stddev();

            case Stat::nu : 
                return nu_.stddev();

            case Stat::distance : 
                return distance_.stddev();

            case Stat::dx : 
                return dx_.stddev();

            case Stat::dy : 
                return dy_.stddev();

            case Stat::dz : 
                return dz_.stddev();

            case Stat::wall :
            {
                if (walls_.size() == 0) return 0;
                Accumulator wall_accumulator;
                for (auto&& wall : walls_)
                    wall_accumulator.accumulate(wall.second);
                return wall_accumulator.stddev();
            }

        }
    }

}
