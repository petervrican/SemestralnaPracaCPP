#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <libds/amt/implicit_sequence.h>
#include "UdajeRiadku.h"

using namespace std;

class Vstup
{
private:
    ifstream aNazovSuboru;
public:
    Vstup(string pNazovSuboru)
    {
        if (!pNazovSuboru.empty())
        {
            this->aNazovSuboru.open(pNazovSuboru, ifstream::in);
        }
    }

    bool nacitajSekvenciu(ds::amt::ImplicitSequence<UdajeRiadku>* pSekvenciaNacitania)
    {
        if (this->aNazovSuboru.is_open())
        {
            string riadok;
            getline(this->aNazovSuboru, riadok);
            while (getline(this->aNazovSuboru, riadok))
            {
                if (!riadok.empty())
                {
                    pSekvenciaNacitania->insertLast().data_ = UdajeRiadku::zCSVRiadku(riadok);
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    ~Vstup()
    {
        if (this->aNazovSuboru.is_open())
        {
            this->aNazovSuboru.close();
        }
    }
};

