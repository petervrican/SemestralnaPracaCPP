#include <algorithm>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <libds/adt/tree.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/table.h>
#include "Vstup.h"
#include "UdajeRiadku.h"
#include "Algoritmus.h"
#include "DorobenieTabulky.h"

//TODO: Duplicity bez zoradenia, zamyslenie sa nad sekvenciou v tabulke, Chyby pri naËÌtavanÌ, Zl˙Ëiù vypisovanie
//Ot·zka: PouûÌvanie dynamickej alok·cie

// Malo by odstr·niù duplicity, ale iba keÔ id˙ za sebou
void odstranDUplicityZaSebou(ds::amt::ImplicitSequence<UdajeRiadku>* pSekvencia)
{
    if (pSekvencia->isEmpty()) return;

    for (size_t i = 1; i < pSekvencia->size() - 1; ) {
        if (pSekvencia->access(i)->data_ == pSekvencia->access(i + 1)->data_)
        {
            pSekvencia->remove(i);
        }
        else {
            ++i;
        }
    }
}

void zobrazMenu() {
    std::cout << "=================== MENU ===================" << std::endl;
    std::cout << "Vyber si, ktor˙ ˙roveÚ chceö testovaù." << std::endl;
    std::cout << "1. Sekvencia" << endl;
    std::cout << "2. Hierarchia" << endl;
    std::cout << "3. Tabuæka" << endl;
    std::cout << "4. <- UkonËiù" << endl;
    std::cout << "Vyber si moûnosù: ";
}

void zobrazMenuOperacie() {
    std::cout << "=================== MENU ===================" << std::endl;
    std::cout << "Vyber si oper·ciu, ktor˙ chceö vykon·vaù." << std::endl;
    std::cout << "1. matchWithAddress" << endl;
    std::cout << "2. matchLifetime" << endl;
    std::cout << "3. matchNextHop" << endl;
    std::cout << "4. <- sp‰ù" << endl;
    std::cout << "Vyber si moûnosù: ";
}

void zobrazMenuTabulky() {
    std::cout << "=================== MENU ===================" << std::endl;
    std::cout << "Vyber si, Ëo chceö urobiù." << std::endl;
    std::cout << "1. Vyhæadaù pomocou next-hop adresy" << endl;
    std::cout << "2. <- sp‰ù" << endl;
    std::cout << "Vyber si moûnosù: ";
}

void zobrazMenuHierarchie(uint8_t *pOktety, int &pIndexOktetu) {
    std::cout << "============ MENU ============"<< std::endl;
    std::cout << "Aktu·lne si na: " << std::endl;
    for (int i = 0; i < 3; i++)
    {
        if (pIndexOktetu < i)
        {
            std::cout << "X.";
            continue;
        }
        std::cout << +pOktety[i] << ".";
    }
    if (pIndexOktetu < 3)
    {
        std::cout << "X" << std::endl;;
    }
    else
    {
        std::cout << +pOktety[3] << std::endl;
    }
    std::cout << "1. Oper·cia" << endl;
    std::cout << "2. Presun  o oktet vyööie" << endl;
    std::cout << "3. Presun o oktet niûöie" << endl;
    std::cout << "4. <- sp‰ù" << endl;
    std::cout << "Vyber si moûnosù: ";
}

// Malo by vyfiltorvaù duplicity, ale iba keÔ id˙ za sebou
void priradPrvkySekvencieHierarchii(ds::amt::ImplicitSequence<UdajeRiadku>* pSekvencia, ds::amt::MultiWayExplicitHierarchy<UdajeRiadku*>* pHierarchia)
{
    ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* koren = (*pHierarchia).accessRoot();
    ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* akt1 = nullptr;
    ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* akt2 = nullptr;
    ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* akt3 = nullptr;
    ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* akt4 = nullptr;

    int predOktet1 = -1, predOktet2 = -1, predOktet3 = -1;

    auto sekHIerIterator = pSekvencia->begin();
    for (; sekHIerIterator != pSekvencia->end(); ++sekHIerIterator)
    {
        UdajeRiadku* aktualnyRiadok = &(*sekHIerIterator);
        uint8_t o1 = UdajeRiadku::dajOktet(aktualnyRiadok->dajPrefixAdresu(), 0);
        uint8_t o2 = UdajeRiadku::dajOktet(aktualnyRiadok->dajPrefixAdresu(), 1);
        uint8_t o3 = UdajeRiadku::dajOktet(aktualnyRiadok->dajPrefixAdresu(), 2);
        uint8_t o4 = UdajeRiadku::dajOktet(aktualnyRiadok->dajPrefixAdresu(), 3);

        if (o1 != predOktet1) {
            akt1 = &(*pHierarchia).emplaceSon(*koren, 0);
            akt1->data_ = nullptr;
            akt2 = akt3 = akt4 = nullptr;
            //std::cout << static_cast<int>(o1) << " Oktet 1" << std::endl;
        }
        if (o2 != predOktet2 || o1 != predOktet1) {
            akt2 = &(*pHierarchia).emplaceSon(*akt1, 0);
            akt2->data_ = nullptr;
            akt3 = akt4 = nullptr;
            //std::cout << static_cast<int>(o2) << " Oktet 2" << std::endl;
        }
        if (o3 != predOktet3 || o2 != predOktet2 || o1 != predOktet1) {
            akt3 = &(*pHierarchia).emplaceSon(*akt2, 0);
            akt3->data_ = nullptr;
            akt4 = nullptr;
            //std::cout << static_cast<int>(o3) << " Oktet 3" << std::endl;
        }
        if (akt4 == nullptr)
        {
            akt4 = &(*pHierarchia).emplaceSon(*akt3, 0);
            akt4->data_ = aktualnyRiadok;
        }
        else
        {
            if ((*(akt4->data_)) != *aktualnyRiadok)
            {
                akt4 = &(*pHierarchia).emplaceSon(*akt3, (*pHierarchia).degree(*akt3));
                akt4->data_ = aktualnyRiadok;
            }
        }
        //std::cout << akt4->data_ << " Oktet 4" << std::endl;

        predOktet1 = o1;
        predOktet2 = o2;
        predOktet3 = o3;
    }
}

void priradPrvkyTabulke(ds::amt::ImplicitSequence<UdajeRiadku>* pSekvencia, ds::adt::HashTableMyAddition<AdresaIPV4, UdajeRiadku*>* pTabulka) 
{
    auto sekHIerIterator = pSekvencia->begin();
    for (; sekHIerIterator != pSekvencia->end(); ++(sekHIerIterator))
    {
        (*pTabulka).insertValue((*sekHIerIterator).dajNextHop(), &(*sekHIerIterator));
    }
}

using SeqDataType = ds::amt::ImplicitSequence<UdajeRiadku>::BlockType::DataT;

int main()
{
    initHeapMonitor();
	SetConsoleCP(1250);
	SetConsoleOutputCP(1250);

    Algoritmus algoritmus;
	Vstup vstup("RTUprava2.csv");

    ds::amt::ImplicitSequence<UdajeRiadku>* suborUdajovRiadku = new ds::amt::ImplicitSequence<UdajeRiadku>();

    if (vstup.nacitajSekvenciu(suborUdajovRiadku))
    {
        odstranDUplicityZaSebou(suborUdajovRiadku);

        ds::amt::ImplicitSequence<UdajeRiadku>* vyfiltrovaneUdajeRiadkov = new ds::amt::ImplicitSequence<UdajeRiadku>();
        ds::amt::MultiWayExplicitHierarchy<UdajeRiadku*>* hierarchia = new ds::amt::MultiWayExplicitHierarchy<UdajeRiadku*>();
        (*hierarchia).emplaceRoot();
        ds::adt::HashTableMyAddition<AdresaIPV4, UdajeRiadku*>* tabulkaNexHopov = new ds::adt::HashTableMyAddition<AdresaIPV4, UdajeRiadku*>();

        priradPrvkySekvencieHierarchii(suborUdajovRiadku, hierarchia);
        priradPrvkyTabulke(suborUdajovRiadku, tabulkaNexHopov);

        int volbaUrovne;

        do 
        {
            zobrazMenu();
            std::cin >> volbaUrovne;

            switch (volbaUrovne)
            {
            case 1:
            {

                int volbaOperacie = 0;
                do
                {
                    zobrazMenuOperacie();
                    std::cin >> volbaOperacie;

                    vyfiltrovaneUdajeRiadkov->clear();

                    if (volbaOperacie == 1)
                    {
                        std::string ipAdresaVstupRetazec = "";
                        std::cout << "Zadaj IP: " << std::endl;
                        std::cin >> ipAdresaVstupRetazec;
                        uint32_t ipAdresaVstup = UdajeRiadku::stringNaIp(ipAdresaVstupRetazec);

                        algoritmus.algoritmus(suborUdajovRiadku->begin(),
                            suborUdajovRiadku->end(),
                            [&](const SeqDataType pUr) {
                                uint32_t sietovaAdresaZaznamu = pUr.dajPrefixAdresu() & pUr.dajPrefixMasku();
                                uint32_t sietovaAdresavstupu = ipAdresaVstup & pUr.dajPrefixMasku();
                                return sietovaAdresaZaznamu == sietovaAdresavstupu;
                            },
                            [&](SeqDataType pUr, ds::amt::ImplicitSequence<UdajeRiadku>* pVysledok)
                            {
                                pVysledok->insertLast().data_ = pUr;
                            },
                            vyfiltrovaneUdajeRiadkov);

                        auto zobrazVysledkyIterator = vyfiltrovaneUdajeRiadkov->begin();
                        for (; zobrazVysledkyIterator != vyfiltrovaneUdajeRiadkov->end(); ++zobrazVysledkyIterator)
                        {
                            (*zobrazVysledkyIterator).vypis();
                        }
                    }
                    else if (volbaOperacie == 2)
                    {
                        std::string zaciatokRozsahuVstupRetazec = "";
                        std::string koniecRozsahuVstupRetazec = "";
                        std::cout << "Zadaj zaËiatok rozsahu: " << std::endl;
                        std::cin >> zaciatokRozsahuVstupRetazec;
                        std::cout << "Zadaj koniec rozsahu: " << std::endl;
                        std::cin >> koniecRozsahuVstupRetazec;
                        uint64_t zaciatokRozsahuVstup = std::stoi(zaciatokRozsahuVstupRetazec);
                        uint64_t koniecRozsahuVstup = std::stoi(koniecRozsahuVstupRetazec);

                        algoritmus.algoritmus(suborUdajovRiadku->begin(),
                            suborUdajovRiadku->end(),
                            [&](const SeqDataType pUr) {
                                return pUr.dajCas() >= zaciatokRozsahuVstup && pUr.dajCas() <= koniecRozsahuVstup;
                            },
                            [&](SeqDataType pUr, ds::amt::ImplicitSequence<UdajeRiadku>* pVysledok)
                            {
                                pVysledok->insertLast().data_ = pUr;
                            },
                            vyfiltrovaneUdajeRiadkov);

                        auto zobrazVysledkyIterator = vyfiltrovaneUdajeRiadkov->begin();
                        for (; zobrazVysledkyIterator != vyfiltrovaneUdajeRiadkov->end(); ++zobrazVysledkyIterator)
                        {
                            (*zobrazVysledkyIterator).vypis();
                        }
                    }
                    else if (volbaOperacie == 3)
                    {
                        std::string ipAdresaVstupRetazec = "";
                        std::cout << "Zadaj next hop IP: " << std::endl;
                        std::cin >> ipAdresaVstupRetazec;
                        uint32_t ipAdresaVstup = UdajeRiadku::stringNaIp(ipAdresaVstupRetazec);

                        algoritmus.algoritmus(suborUdajovRiadku->begin(),
                            suborUdajovRiadku->end(),
                            [&](const SeqDataType pUr) {
                                return pUr.dajNextHop() == ipAdresaVstup;
                            },
                            [&](SeqDataType pUr, ds::amt::ImplicitSequence<UdajeRiadku>* pVysledok)
                            {
                                pVysledok->insertLast().data_ = pUr;
                            },
                            vyfiltrovaneUdajeRiadkov);

                        auto zobrazVysledkyIterator = vyfiltrovaneUdajeRiadkov->begin();
                        for (; zobrazVysledkyIterator != vyfiltrovaneUdajeRiadkov->end(); ++zobrazVysledkyIterator)
                        {
                            (*zobrazVysledkyIterator).vypis();
                        }
                    }
                    else if (volbaOperacie == 4)
                    {
                        break;
                    }
                } while (volbaOperacie != 4);
                break;
            }
            case 2:
            {
                uint8_t oktetyZobrazenia[4] = { 0, 0, 0, 0 };
                int indexOktetuZobrazenia = -1;

                ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* zVrcholIterovania = (*hierarchia).accessRoot();

                int volbaMoznostiHierarchie;
                do
                {
                    zobrazMenuHierarchie(oktetyZobrazenia, indexOktetuZobrazenia);
                    std::cin >> volbaMoznostiHierarchie;

                    vyfiltrovaneUdajeRiadkov->clear();

                    switch (volbaMoznostiHierarchie) {
                    case 1:
                    {
                        ds::amt::MultiWayExplicitHierarchy<UdajeRiadku*>::PreOrderHierarchyIterator prechadzanieHierIterator{ hierarchia, zVrcholIterovania };
                        ++prechadzanieHierIterator;

                        int volbaOperacie = 0;
                        zobrazMenuOperacie();
                        std::cin >> volbaOperacie;

                        if (volbaOperacie == 1)
                        {
                            std::string ipAdresaVstupRetazec = "";
                            std::cout << "Zadaj IP: " << std::endl;
                            std::cin >> ipAdresaVstupRetazec;
                            uint32_t ipAdresaVstup = UdajeRiadku::stringNaIp(ipAdresaVstupRetazec);

                            algoritmus.algoritmus(prechadzanieHierIterator,
                                hierarchia->endPre(),
                                [&](const UdajeRiadku* pUr) {
                                    if (pUr != nullptr)
                                    {
                                        uint32_t sietovaAdresaZaznamu = pUr->dajPrefixAdresu() & pUr->dajPrefixMasku();
                                        uint32_t sietovaAdresavstupu = ipAdresaVstup & pUr->dajPrefixMasku();
                                        return sietovaAdresaZaznamu == sietovaAdresavstupu;
                                    }
                                    return false;
                                },
                                [&](UdajeRiadku* pUr, ds::amt::ImplicitSequence<UdajeRiadku>* pVysledok)
                                {
                                    pVysledok->insertLast().data_ = *pUr;
                                },
                                vyfiltrovaneUdajeRiadkov);

                            auto zobrazVysledkyIterator = vyfiltrovaneUdajeRiadkov->begin();
                            for (; zobrazVysledkyIterator != vyfiltrovaneUdajeRiadkov->end(); ++zobrazVysledkyIterator)
                            {
                                (*zobrazVysledkyIterator).vypis();
                            }
                        }
                        else if (volbaOperacie == 2)
                        {
                            std::string zaciatokRozsahuVstupRetazec = "";
                            std::string koniecRozsahuVstupRetazec = "";
                            std::cout << "Zadaj zaËiatok rozsahu: " << std::endl;
                            std::cin >> zaciatokRozsahuVstupRetazec;
                            std::cout << "Zadaj koniec rozsahu: " << std::endl;
                            std::cin >> koniecRozsahuVstupRetazec;
                            uint64_t zaciatokRozsahuVstup = std::stoi(zaciatokRozsahuVstupRetazec);
                            uint64_t koniecRozsahuVstup = std::stoi(koniecRozsahuVstupRetazec);

                            algoritmus.algoritmus(prechadzanieHierIterator,
                                hierarchia->endPre(),
                                [&](const UdajeRiadku* pUr) {
                                    if (pUr != nullptr)
                                    {
                                        return pUr->dajCas() >= zaciatokRozsahuVstup && pUr->dajCas() <= koniecRozsahuVstup;
                                    }
                                    return false;
                                },
                                [&](UdajeRiadku* pUr, ds::amt::ImplicitSequence<UdajeRiadku>* pVysledok)
                                {
                                    pVysledok->insertLast().data_ = *pUr;
                                },
                                vyfiltrovaneUdajeRiadkov);

                            auto zobrazVysledkyIterator = vyfiltrovaneUdajeRiadkov->begin();
                            for (; zobrazVysledkyIterator != vyfiltrovaneUdajeRiadkov->end(); ++zobrazVysledkyIterator)
                            {
                                (*zobrazVysledkyIterator).vypis();
                            }
                        }
                        else if (volbaOperacie == 3)
                        {
                            std::string ipAdresaVstupRetazec = "";
                            std::cout << "Zadaj next hop IP: " << std::endl;
                            std::cin >> ipAdresaVstupRetazec;
                            uint32_t ipAdresaVstup = UdajeRiadku::stringNaIp(ipAdresaVstupRetazec);

                            algoritmus.algoritmus(prechadzanieHierIterator,
                                hierarchia->endPre(),
                                [&](const UdajeRiadku* pUr) {
                                    if (pUr != nullptr)
                                    {
                                        return pUr->dajNextHop() == ipAdresaVstup;
                                    }
                                    return false;
                                },
                                [&](UdajeRiadku* pUr, ds::amt::ImplicitSequence<UdajeRiadku>* pVysledok)
                                {
                                    pVysledok->insertLast().data_ = *pUr;
                                },
                                vyfiltrovaneUdajeRiadkov);

                            auto zobrazVysledkyIterator = vyfiltrovaneUdajeRiadkov->begin();
                            for (; zobrazVysledkyIterator != vyfiltrovaneUdajeRiadkov->end(); ++zobrazVysledkyIterator)
                            {
                                (*zobrazVysledkyIterator).vypis();
                            }
                        }
                        else if (volbaOperacie == 4)
                        {
                            break;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (indexOktetuZobrazenia != -1)
                        {
                            zVrcholIterovania = hierarchia->accessParent(*zVrcholIterovania);
                            oktetyZobrazenia[indexOktetuZobrazenia] = -1;
                            --indexOktetuZobrazenia;
                        }
                        else
                        {
                            std::cout << "Vyööie sa uû Ìsù ned·." << std::endl;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (indexOktetuZobrazenia != 3)
                        {
                            ds::amt::ImplicitSequence<uint8_t> moznostiOktetov;
                            ds::amt::MultiWayExplicitHierarchyBlock<UdajeRiadku*>* koncovySyn;
                            for (auto synIterator = zVrcholIterovania->sons_->begin(); synIterator != zVrcholIterovania->sons_->end(); ++synIterator)
                            {
                                koncovySyn = (*synIterator);

                                while (koncovySyn->data_ == nullptr)
                                {
                                    koncovySyn = hierarchia->accessSon(*koncovySyn, 0);
                                }
                                moznostiOktetov.insertLast().data_ = UdajeRiadku::dajOktet(koncovySyn->data_->dajPrefixAdresu(), indexOktetuZobrazenia + 1);
                            }

                            int volbaOktetu = 0;
                            for (auto moznostIterator = moznostiOktetov.begin(); moznostIterator != moznostiOktetov.end(); ++moznostIterator)
                            {
                                std::cout << +(*moznostIterator) << std::endl;
                            }
                            ds::amt::MemoryBlock<uint8_t>* najdene;
                            std::cout << "Vyber si vrchol." << std::endl;
                            do
                            {
                                std::cin >> volbaOktetu;

                                najdene = moznostiOktetov.findBlockWithProperty(
                                    [&](ds::amt::MemoryBlock<uint8_t>* pPrvok)
                                    {
                                        return pPrvok->data_ == volbaOktetu;
                                    });
                                int najdenyIndex = moznostiOktetov.calculateIndex(*najdene);

                                if (najdene == nullptr)
                                {
                                    std::cout << "Tak·to moûnmosù neexistuje, sk˙s znova." << std::endl;
                                }
                                else
                                {
                                    zVrcholIterovania = hierarchia->accessSon(*zVrcholIterovania, najdenyIndex);
                                    ++indexOktetuZobrazenia;
                                    oktetyZobrazenia[indexOktetuZobrazenia] = volbaOktetu;
                                }
                            } while (najdene == nullptr);
                        }
                        else
                        {
                            std::cout << "Niûöie sa uû Ìsù ned·." << std::endl;
                        }
                        break;
                    }
                    case 4:
                        std::cout << "Vraciam sa sp‰ù..." << std::endl;
                        break;
                    default:
                        std::cout << "Zl· voæba, prosÌm sk˙s znova." << std::endl;
                    }
                } while (volbaMoznostiHierarchie != 4);
                break;
            }
            case 3:
            {
                int volbaMoznostiTabulky;

                do
                {
                    zobrazMenuTabulky();
                    std::cin >> volbaMoznostiTabulky;

                    switch (volbaMoznostiTabulky)
                    {
                    case 1:
                    {
                        auto tabHIerIterator = tabulkaNexHopov->begin();
                        for (; tabHIerIterator != tabulkaNexHopov->end(); ++tabHIerIterator)
                        {
                            std::cout << UdajeRiadku::ipNaString((*tabHIerIterator).key_) << std::endl;
                        }

                        std::string ipAdresaVstupRetazec = "";
                        std::cout << "Zadaj next-hop IP z ponuky: " << std::endl;
                        std::cin >> ipAdresaVstupRetazec;
                        AdresaIPV4 ipAdresaVstup = UdajeRiadku::stringNaIp(ipAdresaVstupRetazec);

                        ds::adt::HashTableMyAddition<AdresaIPV4, UdajeRiadku*>::Sequence** sekvenciaNexHopov = nullptr;
                        if (tabulkaNexHopov->tryFind(ipAdresaVstup, sekvenciaNexHopov)) {
                            auto tabHIerIterator = (**sekvenciaNexHopov).begin();
                            for (; tabHIerIterator != (**sekvenciaNexHopov).end(); ++tabHIerIterator)
                            {
                                (*tabHIerIterator)->vypis();
                            }
                        }
                        else {
                            std::cout << "Zadan˝ next-hop neexistuje." << std::endl;
                        }
                        break;
                    }
                    case 2:
                    {
                        std::cout << "Vraciam sa sp‰ù..." << std::endl;
                        break;
                    }
                    default:
                        std::cout << "Zl· voæba, prosÌm sk˙s znova." << std::endl;
                        break;
                    }
                } while (volbaMoznostiTabulky != 2);

                break;
            }
            case 4:
            {
                std::cout << "UkonËujem..." << std::endl;
                break;
            }
            default:
                std::cout << "Zl· voæba, prosÌm sk˙s znova." << std::endl;
            }
        } while (volbaUrovne != 4);

        auto tabHIerIterator = tabulkaNexHopov->begin();
        for (; tabHIerIterator != tabulkaNexHopov->end(); ++tabHIerIterator)
        {
            delete (*tabHIerIterator).data_;
        }
        delete tabulkaNexHopov;
        delete hierarchia;
        delete vyfiltrovaneUdajeRiadkov;
    }
    else
    {
        std::cout << "Chyba: Nemoûno otvoriù s˙bor." << std::endl;
    }

    delete suborUdajovRiadku;

    return 0;
}
