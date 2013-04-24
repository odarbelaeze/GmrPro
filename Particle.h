#ifndef PARTICLE_H_
#define PARTICLE_H_ 

#include <valarray>
#include <vector>

namespace gmr
{
    typedef std::valarray<double> darray;
    enum class Spin { Up, Down };
    enum class Specie { Ion, Electron };

    class Particle
    {
    public:
        Particle();
        Particle (darray, Spin, Specie);
        ~Particle();

        darray getPosition () const;
        darray getOldPosition () const;
        Spin getSpin () const;
        double getCharge() const;
        Specie getSpecie () const;
        std::vector<Particle*> getNbh () const;

        void setPosition (const darray&);
        void setOldPosition (const darray&);
        void setSpin (const Spin&);
        void setCharge(double);
        void setSpecie (const Specie&);
        void setNbh (const std::vector<Particle*>&);

    private:
        darray position_;
        darray oldPosition_;
        Spin spin_;
        double charge_;
        Specie specie_;
        std::vector<Particle*> nbh_;
    };

    int operator* (const Spin&, const Spin&);
    Spin operator- (const Spin&);


    template <typename T>
    darray fmod(const darray& da, std::initializer_list<T> dim_list)
    {
        if (da.size() != dim_list.size())
            throw std::exception();

        darray answer(da.size());
        for (int i = 0; i < dim_list.size(); ++i)
        {
            answer[i] = std::fmod(da[i], (double)(*(dim_list.begin() + i)));
            if (answer[i] < 0) answer[i] = answer[i] + (double)(*(dim_list.begin() + i));
        }
        
        return answer;
    }

}

#endif