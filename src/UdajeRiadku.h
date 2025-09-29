#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cmath> // kvôli std::pow

using namespace std;

using CasSekundy = uint64_t;
using AdresaIPV4 = uint32_t;
using MaskaIPV4 = uint32_t;

class UdajeRiadku
{
private:
    CasSekundy aCas;
    AdresaIPV4 aPrefixAdresa;
    MaskaIPV4 aPrefixMaska;
    AdresaIPV4 aNextHop;

    static constexpr bool POUZI_TRIEDNU_IP_ADRESU = true;

public:

    UdajeRiadku(CasSekundy pCas, AdresaIPV4 pPrefixAdresa, MaskaIPV4 pPrefixMaska, AdresaIPV4 pNextHop)
        : aCas(pCas), aPrefixAdresa(pPrefixAdresa), aPrefixMaska(pPrefixMaska), aNextHop(pNextHop)
    {
    }

    UdajeRiadku()
        : aCas(0), aPrefixAdresa(0), aPrefixMaska(0), aNextHop(0)
    {
    }

    ~UdajeRiadku() {}

    UdajeRiadku(const UdajeRiadku& pDalsi)
        : aCas(pDalsi.aCas), aPrefixAdresa(pDalsi.aPrefixAdresa),
        aPrefixMaska(pDalsi.aPrefixMaska), aNextHop(pDalsi.aNextHop)
    {
    }

    // Gettery
    CasSekundy dajCas() const { return this->aCas; }
    AdresaIPV4 dajPrefixAdresu() const { return this->aPrefixAdresa; }
    MaskaIPV4 dajPrefixMasku() const { return this->aPrefixMaska; }
    AdresaIPV4 dajNextHop() const { return this->aNextHop; }

    static uint8_t dajOktet(const AdresaIPV4 pAdresa, int pIndex)
    {
        return (pAdresa >> (8 * (3 - pIndex))) & 0xFF;
    }

    void vypis() const
    {
        cout << "Èas: " << casNaString(this->aCas) << endl;
        cout << "Prefix adresa: " << ipNaString(this->aPrefixAdresa) << endl;
        cout << "Prefix maska: " << ipNaString(this->aPrefixMaska) << endl;
        cout << "Next hop: " << ipNaString(this->aNextHop) << endl;
    }

    static UdajeRiadku zCSVRiadku(const string& pRiadok)
    {
        size_t zaciatok = 0;
        size_t koniec = pRiadok.find(';');

        zaciatok = koniec + 1;
        koniec = pRiadok.find(';', zaciatok);
        CasSekundy cas = stoull(pRiadok.substr(zaciatok, koniec - zaciatok));

        zaciatok = koniec + 1;
        koniec = pRiadok.find(';', zaciatok);
        AdresaIPV4 prefixAdresa = stringNaIp(pRiadok.substr(zaciatok, koniec - zaciatok));

        zaciatok = koniec + 1;
        koniec = pRiadok.find(';', zaciatok);
        MaskaIPV4 prefixMaska = 0;

        if (zaciatok == koniec)
        {
            if (UdajeRiadku::POUZI_TRIEDNU_IP_ADRESU)
            {
                prefixMaska = dajMaskuPodlaTriedyIp(prefixAdresa);
            }
            else
            {
                throw invalid_argument("Chýba pole masky.");
            }
        }
        else
        {
            prefixMaska = static_cast<uint32_t>(~(static_cast<uint32_t>(pow(2, 32 - stoi(pRiadok.substr(zaciatok, koniec - zaciatok)))) - 1));
        }

        koniec = pRiadok.find('v', zaciatok);
        zaciatok = koniec + 3; // preskoèí 'via'
        string nextHopRetazec = pRiadok.substr(zaciatok);
        AdresaIPV4 nextHop = stringNaIp(nextHopRetazec);

        return UdajeRiadku(cas, prefixAdresa, prefixMaska, nextHop);
    }

    static AdresaIPV4 stringNaIp(const string& pRetazec)
    {
        AdresaIPV4 ip = 0;
        stringstream ss(pRetazec);
        string bajt;
        int i = 3;
        while (getline(ss, bajt, '.'))
        {
            ip |= (stoi(bajt) << (i * 8));
            i--;
        }
        return ip;
    }

    static string ipNaString(AdresaIPV4 pIp)
    {
        ostringstream oss;
        for (int i = 3; i >= 0; --i)
        {
            oss << ((pIp >> (i * 8)) & 0xFF);
            if (i != 0)
            {
                oss << ".";
            }
        }
        return oss.str();
    }

private:
    static string casNaString(CasSekundy pCas)
    {
        return to_string(pCas) + " sekúnd";
    }

    static MaskaIPV4 dajMaskuPodlaTriedyIp(AdresaIPV4 pIp)
    {
        uint8_t prvyOktet = (pIp >> 24) & 0xFF;
        if (prvyOktet < 128) return 0xFF000000;
        if (prvyOktet < 192) return 0xFFFF0000;
        return 0xFFFFFF00;
    }

    friend bool operator==(const UdajeRiadku& pLavy, const UdajeRiadku& pPravy)
    {
        return pLavy.aCas == pPravy.aCas &&
            pLavy.aPrefixAdresa == pPravy.aPrefixAdresa &&
            pLavy.aPrefixMaska == pPravy.aPrefixMaska &&
            pLavy.aNextHop == pPravy.aNextHop;
    }

    friend bool operator!=(const UdajeRiadku& pLavy, const UdajeRiadku& pPravy)
    {
        return !(pLavy == pPravy);
    }
};
