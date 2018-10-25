 /* 
 * File:   Sudoku.cpp
 * Author: Sabetay Toros 27 Sep 18.15
 *
 * Created on 06 August 2018, 18:05
 */
/******************************************************************************/

#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <array>
#include <iterator>
#include <algorithm>
#include <memory>
#include <experimental/optional>
#include <tuple>  

#include "Sudoku.h"
static Uint Counter = 0;
/******************************************************************************/
auto mxelem = [&](auto& l, auto& r) {
    if (l == nine) return true;
    return r < nine && l < r;
};

const char* str[] = {" Row Details ", " Column Details "};

/******************************************************************************/
ostream& operator<<(ostream& os, const Uint* ptr) {
    if (ptr == nullptr) os << "0 ";
    else os << *ptr << " ";
    return os;
}
/******************************************************************************/
ostream& operator<<(ostream& os, const TCell& It) {
    return os << '\t' << " Digit No " << It.Digit.No << It.Lct // << " r " << It.r << " c " << It.c 
            << " ta " << It.ta << " tr " << It.tr << " tc " << It.tc << endl;
}
/******************************************************************/
ostream& operator<<(ostream& os, const TLoc& v) {
    return os << " Row " << v.x << " Column " << v.y;
}
/******************************************************************/
ostream& operator<<(ostream& os, const TBox& v) {
    os << " Triple Detail No " << v.tn << " " << v.Lct << endl;
    os << " Ribbon No " << v.Rbn.x << " " << v.Rbn.y << endl;
    os << " Triple Digit Items " << v.Cells.size() << endl;
    for (const auto& e : v.Cells)
        os << *e;
    os << endl;
}
/******************************************************************/
ostream& operator<<(ostream& os, const TDigit& v) {
    os << "CellNo " << v.No << endl;
    for (auto& e : v.Cells)
        cout << *e;
    return os;
}
/******************************************************************/
ostream& operator<<(ostream& os, const TLine& v) {
    os << str[v.lt] << v.No << endl;
    for (auto& e : v.Cells)
        cout << *e;
    return os;
}
/******************************************************************/
ostream& operator<<(ostream& os, const TRibbon& v) {
    os << str[v.lt] << " Ribbons " << v.No << endl;
    for (auto& r : v.RvLines) {
        os << *r << endl;
    }
    os << " Triples " << endl;
    for (auto& t : v.vpBoxes)
        os << *t << endl;

    os << " Counts " << endl;
    for (const auto& e : v.Counts)
        os << e << " ";
    os << endl;
    return os;
}
/******************************************************************************/
namespace SudokuApp {
    TvUint vint{ 0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u};
    bool intrm = false;
/**************************************************************************/
    void PrintCounts(TCounts& Counts) {
        cout << "Max Counts  CelNo Row Column Triple" << endl;
        for (auto& a : Counts) {
            for (auto& e : a)
                cout << " " << e;
            cout << endl;
        }
    }
 /**************************************************************************/
    void PrintLine(auto& e) {
        for (const auto& ptr : e)
            cout << ptr;
        cout << endl;
    }
/**************************************************************************/
    void PrintDoubleArray(const auto& s, const auto& v) {
        cout << s << endl;
        int r = 0;
        for (auto& e : v) {
            cout << r << '\t';
            PrintLine(e);
            ++r;
        }
        cout << endl;
    }
/**************************************************************************/
    void PrintBoxMatrix(auto& Box) {
        cout << " Box Matrix " ;
        for (auto& a : Box) {
            for (auto& e : a)
                cout << e;
        }
        cout << endl;
    }
/**************************************************************************/
    void PrintDigitDetails(auto& Cells) {
        cout << " Digit Details " << endl << endl;
        for (auto& e : Cells) {
            cout << e;
        }
    }
/**************************************************************************/
    void PrintRows(TvsLine& Rows) {
        for (auto k = 0; k < two; ++k) {
            cout << endl << (k == 0 ? " Rows " : " Columns ") << endl;
            for (auto& e : Rows[k])
                cout << *e << endl;
        }
    }
/**************************************************************************/
    PrintRibbons(TvsRibbon& Ribbons) {
        for (auto k = 0; k < two; ++k) {
            cout << endl << (k == 0 ? " Row  Ribbons " : " Column Ribbons ") << endl;
            for (auto& e : Ribbons[k])
                cout << *e << endl;
        }
    }
}
using namespace SudokuApp;
/*******************************************************************************/
experimental::optional<Tsfstream> GetFileHandle() {
    Tsfstream fs { make_shared<fstream> () };
    fs->open("SudokuTest.txt",ios::in);
    if (fs->fail()) {
        cout << " SudokuTest.txt File not found  " << endl;
        return {};
    } 
    return fs;
}
/*******************************************************************************/
TSdkData::TSdkData(string s, fstream& fs) : Identity(move(s)) {
    auto v { 0u };
    for(auto  j = 0u; j < eightyone; ++j) {
        fs >> v;
        Data.emplace_back(v);
        if (fs.eof() ) return;
    }    
    string flsh;
    getline(fs, flsh); // 
    State = true;   
}
/*******************************************************************************/
experimental::optional<TSdkData> GetData(fstream* fs) {
    std::string id;
    for (;;) {
        getline(*fs, id);
        if (fs->eof())  
            return {};
        if (id[0] == '*') break;
        cout << " Identity first char must be ' * ' not " <<  id [0] << endl;
    }
    SudokuApp::intrm = id[1] == ' ' ? false : true;
    TSdkData Data(id, *fs);
    if (Data.State) return Data;
    return {};
}
/*******************************************************************************/
int main(int argc, char** argv) {
    experimental::optional<Tsfstream> fs =  GetFileHandle();
    if (fs) {
        const Tsfstream& sfs = fs.value();
        fstream *fp = sfs.get();
        for(;;) {
            experimental::optional<TSdkData> SdkData = GetData(sfs.get());
            if (SdkData) 
                TSudoku Sudoku(SdkData.value());
            else break;
        }
    }
    return 0;
}
/*****************************************************************************/
TDigit :: TDigit(TSudoku * o, auto No_) 
        : Osdk { *o }, No(SudokuApp::vint[No_]) { 
} 
/*****************************************************************************/
TSudoku::TSudoku(TSdkData& SdkData) noexcept {
    for (Uint j = 1; j < 10; ++j) Digits.emplace_back(this, j);
    for (Uint j = 0; j < nine; ++j) Boxes.emplace_back(this, j);

    // Settomg Rows & Collums
    auto it = Mtrx.begin();
    auto t = ltr;
    for (Uint k = 0; k < two; ++k) {
        Rows.emplace_back();
        auto& r = Rows.back();
        for (Uint j = 0; j < nine; ++j, ++it)
            r.emplace_back(make_shared<TLine>(this, it, j, t));
        t = ltc;
        it = Clmn.begin();
    }
    // Ribbons
    t = ltr;
    for (auto k = 0u; k < 2u; ++k) {
        Ribbons.emplace_back();
        auto& r{ Ribbons.back()};
        for (Uint j = 0; j < tree; ++j)
            r.emplace_back(make_shared<TRibbon>(this, j, t));
        t = ltc;
    }
    auto c = 0;
    Uint i{ 0};
    Uint j{ 0};
    for (auto& e : SdkData.Data) {
        TLoc cLoc{ c / nine, c % nine};
        if (e > 0) {
            auto n = e - 1;
            Digits[n].GenerateItem(cLoc, true);
        }
        ++c;
    }
    PrintDoubleArray(SdkData.Identity, Mtrx);
    if (intrm) PrintCounts(Counts);
    auto sz = Cells.size();
    auto nsz = sz;
    for (;;) {
        SelectaSolution();
        nsz = Cells.size();
        if (nsz == eightyone || nsz - sz == 0 ) break;
        sz = nsz;
        if (intrm) {
            cout << " size " << Cells.size() << endl;
            PrintDoubleArray("Ara Cikis", Mtrx);
            PrintCounts(Counts);
        }
    }
    if (nsz != eightyone) BackTraceSolution();
    auto flg =  nsz == eightyone || LastTrace == mdTerminate;
    cout << ( flg ? " Puzzle Solved " : " Puzzle not Solved Number of Filled Items ") ; 
    if (!flg) cout << Cells.size() << " ";
    PrintDoubleArray(SdkData.Identity, Mtrx);
    if (!flg) PrintCounts(Counts);
}  
/******************************************************************************/
void TSudoku::BackTraceSolution() {
    for(auto& e : Cells)
        e->isData = true;   
    vector<TsLine>& rw = *Rows.begin();
    Uint Prv { 1 };
    for(const auto& erw : rw) {
        vector<TsLine>& cl = *(next(Rows.begin(), 1));
        for(const auto& ecl : cl) 
            vPosible.emplace_back(make_shared<TPosibility>(erw.get(), ecl.get(), Prv));
    }
    vPosible.begin()->get()->BackTrace();
}
/******************************************************************************/
void TLine::UpdateRowDataDigit(const Uint i, const Uint No) {
    if (No < nine)
        RwData[i] = &Osdk.Digits[No].No;
    else RwData[i] = nullptr;
} 
/******************************************************************************/
void TBox::UpdateRowDataDigit(const TLoc& Lct, const Uint No) {
    if (No < nine)
        srMtrx[Lct.x][Lct.y] = &Osdk.Digits[No].No;  
    else srMtrx[Lct.x][Lct.y] = nullptr;
}
/******************************************************************************/
ostream& operator<<(ostream& os, const TPosibility& v) {
   os << " Cell No " << (v.Next-1) << "  { " << v.Lct.x << " , " << v.Lct.y << " } " ;
   os  << " Row  No " << v.Row.No << " "; PrintLine(v.Row.RwData);
//  os << " Clmn "; PrintLine(v.Clm.RwData);
// PrintBoxMatrix(v.Box.srMtrx);
// cout << "Search Entry ";
}

/******************************************************************************/
bool TLine::RowDatahasaDigit(const Uint Entry)    {
    auto it = find_if(RwData.begin(), RwData.end(), [&](auto& e) { 
        if (e != nullptr)
                return Entry == *e;
        return false;
    });
    return it != RwData.end();
}
/******************************************************************************/
bool TBox::RowMatrixhasaDigit(const Uint Entry) {            
    for (auto& a : srMtrx) {
        for (auto& e : a)
            if (e != nullptr && *e == Entry) return true;
    }
    return false;
}
/******************************************************************************/
TRibbon::TRibbon(TSudoku *O, Uint n, TLineType t) noexcept
        : Osdk { *O }, No{ n}, lt{ t}    {
    auto r = n * 3;
    auto m = (lt == ltr) ? r : n;
    for (Uint i = 0; i < 3; i++) {
        TsLine& sLine = Osdk.Rows[lt][r];
        RvLines.push_back(sLine.get());
        vpBoxes.push_back(&Osdk.Boxes[m]);
        ++r;
        m += (lt == ltr) ? 1 : 3;
    }
}
/******************************************************************************/
TCell::TCell(TDigit* o, TLoc lct, bool d)
        : Digit{*o}, Lct{ move(lct)}, ta{(Lct.x / tree * tree) + Lct.y / tree}
        , tr{Lct.x % tree}, tc{Lct.y % tree}, isData{d} {
    ++Digit.Count;   
    Digit.Osdk.Mtrx[Lct.x][Lct.y] = &Digit.No;
    Digit.Osdk.Clmn[Lct.y][Lct.x] = &Digit.No;
    Digit.tstV[Lct.y] = &Digit.No;
    Digit.tstH[Lct.x] = &Digit.No;
    Digit.Osdk.Boxes[ta].AddCell(*this);
    Digit.Osdk.Rows[ltr][Lct.x]->AddCell(this);
    Digit.Osdk.Rows[ltc][Lct.y]->AddCell(this);
    ++Digit.Osdk.Counts[0][Digit.No - 1];                       // CellNo sayisi
    ++Digit.Osdk.Counts[1][Lct.x];                              // Row Count
    ++Digit.Osdk.Counts[two][Lct.y];                            // ColumnCount
    Digit.Osdk.Cells.push_back(this);
    Digit.Osdk.Inserted = true;
 }
/*************************************************************************C*****/
TBox::TBox(TSudoku *o, Uint tn_) noexcept
        : Osdk { *o }, tn{tn_}, Lct{ tn / tree * tree, tn % tree * tree}
        , Rbn{ tn / tree, tn % tree}    {
}
/******************************************************************************/
void TBox::AddCell(TCell& Cell) {
    srMtrx[Cell.tr][Cell.tc] = &Cell.Digit.No;
    scMtrx[Cell.tc][Cell.tr] = &Cell.Digit.No;
    auto it = find_if(Cells.begin(), Cells.end(), [&] (const TCell* Entry) {
        return Entry->Digit.No > Cell.Digit.No;    });
    Cells.insert(it, &Cell);
    ++Osdk.Counts[tree][Cell.ta]; // Triple Count
    Osdk.ChckR = true;
    for (Uint i = 0; i < two; ++i) {
        Uint j = i == 0 ? Rbn.x : Rbn.y;
        Osdk.Ribbons[i][j]->AddCount(Cell);
    }
    Osdk.Inserted = true;
}
/****************************************************************************/
void TDigit::GenerateItem(TLoc& lct, bool Dflt) {
    Cells.emplace_back(make_shared<TCell>(this, lct, Dflt));
}
/******************************************************************/
auto TDigit::FillTestRowWithNonCellValues(auto MtrxLn, auto& tstV, Uint mr, TLineType t) {
   TRw ntstV = {nullptr}; // new test Vector
    auto f = false;
    Uint tc{ 0};
    auto im = MtrxLn->begin(); //   auto im = *MtrxLn;
    copy(tstV.begin(), tstV.end(), ntstV.begin());
    auto ci = ntstV.end();
    for_each(ntstV.begin(), ntstV.end(), [&](auto& e) {
        if (e == nullptr)
            e = *im;
        ++im;
    });
    auto it = ntstV.begin();
    for (size_t i = 0; i < two; i++) {
        it = find_if(it, ntstV.end(), [] (auto& e) {
            return e == nullptr; });
        if (it != ntstV.end()) {
            if (i == 0) {
                tc = (unsigned) distance(ntstV.begin(), it);
                ++it;
            } else return false;
        }
    }
    auto ip { mr > Cells.size() ? Cells.end() : next(Cells.begin(), mr) };
    TLoc nl { t == ltr ? mr : tc , t == ltr ? tc : mr };
    Cells.insert(ip, make_shared<TCell>(this, nl, false));
    if (Cells.size() == eight) {
        InsertFinalCell();
        return true;
    }
    return Osdk.Inserted;
}
/******************************************************************/
bool TDigit::InsertNewItem(TLoc& lc) {
    auto ip = find_if(Cells.begin(), Cells.end(), [&] (auto& e) {
        return e->Lct.x > lc.x; });
    ip = Cells.insert(ip, make_shared<TCell>(this, lc, false));
    if (Osdk.Cells.size() == eightyone) return true;
    TCell& Cell = *ip->get();  
    for (int i = 0; i < two; ++ i) {
        auto j = i == 0 ? Cell.Lct.x : Cell.Lct.y;
        auto& Line = *Osdk.Rows[i][j];
        if (Line.Cells.size() == eight) Line.InsertFinalCell();
    }
    if (Osdk.Boxes[Cell.ta].Cells.size() == eight)
        Osdk.Boxes[Cell.ta].InsertFinalCell();
    for (Uint i = 0; i < two; i++) {
        Uint j = (i == 0) ? Osdk.Boxes[Cell.ta].Rbn.x : Osdk.Boxes[Cell.ta].Rbn.y;
            Osdk.Ribbons[i][j]->CheckMissingOtherCell(Cell);
        if (Osdk.Cells.size() == eightyone) return  true;
    }
    return false;
}
/***************************************************************************/
void TDigit::InsertFinalCell() {
    auto ih = find_if(tstH.begin(), tstH.end(), [](auto& e) {
        return e == nullptr; });
    auto iv = find_if(tstV.begin(), tstV.end(), [](auto& e) {
        return e == nullptr; });
    TLoc Lct{ (unsigned) distance(tstH.begin(), ih), (unsigned) distance(tstV.begin(), iv)};
    Cells.insert(next(Cells.begin(), Lct.x), make_shared<TCell>(this, Lct, false));
}
/******************************************************************/
void TDigit::FindMissingRow(auto& Vect, Uint& mr) {
    auto it = next(Vect.begin(), mr);
    it = find_if(it, Vect.end(), [&] (const auto& e) { return e == nullptr; } );
    mr = distance(Vect.begin(), it);
}
const char *cstr[] = {"Cells", "Rows", "Column", "Triple"};
/******************************************************************************/
auto MxCellItem = [&] (auto& l, auto& r) {
    if (l.Cells.size() == nine) return true;
    return (l.Cells.size() < r.Cells.size() && r.Cells.size() < nine);
};

/******************************************************************************/
void TSudoku::DigitSolution() {
    auto MxCell = Digits.begin();
    while (MxCell != Digits.end()) {
        MxCell = max_element(MxCell, Digits.end(), MxCellItem);
        if (MxCell != Digits.end() && MxCell->Cells.size() > 5) {
            MxCell->Solution();
            if (Inserted) break;
            ++MxCell;
        } else break;
    }
}
/******************************************************************************/
void TDigit::Solution() {
    for (Uint i = 0; i < two; ++i) {
        Uint mr{0};
        while (mr < nine) {
            FindMissingRow((i==0 ? tstH : tstV), mr); // If Fitted Itm insertion position
            if (mr < nine) {
                if (FillTestRowWithNonCellValues(i == 0 ? next(Osdk.Mtrx.begin(), mr) : next(Osdk.Clmn.begin(), mr),
                    i == 0 ? tstV : tstH , mr, static_cast<TLineType>  ( i ) )) {
                    return;
                }
                ++mr;
            }
        } 
    }
}
/******************************************************************************/
void TSudoku::SelectaSolution() {
    Inserted = false;
    using MxPair = pair<Uint *, Uint >; // first { Digit, Row, Col, Trp } , second Maxlar
    vector<MxPair> vMxPair;
    Uint c {0};
    for (auto& a : Counts) {
        vMxPair.emplace_back(max_element(a.begin(), a.end(), mxelem), c);
        ++c;
    }
    sort(vMxPair.begin(), vMxPair.end(), [](const auto& l, const auto& r) {
        return *l.first > *r.first; });
    auto sz = Cells.size();
    if (intrm) {
        cout << " Maximum Counts " << endl;
        for (auto& e : vMxPair) 
            if (intrm) cout << e.first << " " << " { " << cstr[e.second] << " } " << endl;
    }

    for (auto& e : vMxPair) {
        if (e.second == 0) 
            DigitSolution();
        else if (e.second > 0 && e.second < tree) 
            LineSolution(e.second);
        else if (e.second == tree) {
            auto i = max_element(Counts[3].begin(), Counts[3].end(), mxelem);
            auto d = distance(Counts[3].begin(), i);
            auto MaxTrp = next(Boxes.begin(), d );
            if (MaxTrp->Cells.size() > 3)
                    MaxTrp->Solution(); 
            if(!Inserted)  {
                for(auto c = 0u; c < nine; ++c) {
                    if (c == d || Boxes[c].Cells.size() < 4) continue;
                    Boxes[c].Solution();
                    if (Inserted) break;
                }
            }
            if (!Inserted) RibbonSolution();
        }        
        auto nsz = Cells.size();        
        if (nsz == eightyone || nsz > sz) return;
        if (intrm) cout << cstr[e.second] << " in Solution Cells could not be inserted  " << endl;
    }
}
/*******************************************************************************/
auto TBox::GenerateChechSubMatrix(TDigit& Digit) {
    TBoxMtrx cMtrx{ nullptr};
    auto itc = cMtrx.begin();
    for (auto& a : srMtrx) {
        auto ita = itc->begin();
        copy(a.begin(), a.end(), ita);
        ++itc;
    }
    for (auto j = 0; j < two; ++j) {
        auto& rb = (j == 0) ? Osdk.Ribbons[0][tn / tree]
                            : Osdk.Ribbons[1][tn % tree];
        for (auto& el : rb->RvLines) {
            TLine& Line { *el };
            auto it = find_if(Line.Cells.begin(), Line.Cells.end(), [&](auto& ec) {
                return ec->Digit.No == Digit.No;  });            
            if (it != Line.Cells.end()) {
                TCell& Cell = **it;
                TLoc nLoc {Cell.Lct.x - Lct.x, Cell.Lct.y - Lct.y};
                auto xy = (j == 0) ? nLoc.x : nLoc.y;
                if (j == 0) {
                    for (auto k = 0; k < 3; ++k)
                        if (cMtrx[xy][k] == nullptr)
                            cMtrx[xy][k] = &Digit.No;
                } else {
                    for (auto k = 0; k < tree; ++k)
                        if (cMtrx[k][xy] == nullptr)
                            cMtrx[k][xy] = &Digit.No;
                }
            }
        }
    }
    return cMtrx;
}
/*******************************************************************************/
experimental::optional<TLoc> TBox::FindInsertionLocation(TBoxMtrx& cMtrx) {
    auto i{ 0};
    auto j{ 0};
    vector <TLoc> vLct;
    for (auto& a : cMtrx) {
        auto it{ a.begin()};
        while (it != a.end()) {
            auto k = 0;
            it = find_if(it, a.end(), [&k](auto& e) { return e == nullptr; });
            if (it != a.end()) {
                vLct.emplace_back(i, distance(a.begin(), it));
                ++it;
            }
        }
        ++i;
    }
    if (vLct.size() == 1) 
        return vLct.back();
    return {};
}

/*******************************************************************************/
auto TBox::Solution(Uint No) {
    TDigit& Digit = *next(Osdk.Digits.begin(), No - 1);
    auto cMtrx = GenerateChechSubMatrix(Digit);                           // Checking Sub Matrix
    experimental::optional<TLoc> fl = FindInsertionLocation(cMtrx);         // SearcchforAviableCell
    if (fl) {
        TLoc nl { fl->x + Lct.x, fl->y + Lct.y};
        Digit.InsertNewItem(nl);
        if (Osdk.Cells.size() == eightyone) return true;
        if (Osdk.Counts[3][tn] == eight ) 
            InsertFinalCell();
        if (Cells.size() > five && Cells.size() < nine)  
            InsertOtherCells();   
        return true;
    }     
    return false;
}
/*******************************************************************************/
void TBox::Solution() {
    auto MisClNo = 1;
    auto it = Cells.begin();
    auto c = 1; 
    while (it != Cells.end() && MisClNo < 10 && Osdk.Cells.size() < eightyone  && Osdk.Counts[3][tn] < nine) {  
        if (it != Cells.end()) {
            if (MisClNo == one) 
                it = Cells.begin();
            it = find_if(it, Cells.end(), [&] (const TCell * Entry) {
                if (MisClNo == Entry->Digit.No)  {
                        ++MisClNo;
                        return false;
                } 
                return true;}  );
        }    
        if (MisClNo < ten) {
            auto endNo = it != Cells.end() ? (*it)->Digit.No : nine;
            for(;;)  {
                if (Solution(MisClNo)) {
                    MisClNo = 1;
                 } else ++MisClNo;
                if (MisClNo >= endNo || MisClNo == 1) break;
            }
            if (c > 4) exit(26);
        }  
    }  
}
/*******************************************************************************/
void TBox::InsertOtherCells() {
    for (auto c = 0; c < 4; ++c) {
        Osdk.ChckR = false;
        for (auto& e : Cells) {
            for (Uint i = 0; i < two; i++) {
                Uint j = (i == 0) ? Rbn.x : Rbn.y;
                Osdk.Ribbons[i][j]->CheckMissingOtherCell(*e);
             }
        }
        if (Osdk.ChckR == false) break;
    }
}
/*******************************************************************************/
void TBox::InsertFinalCell() {
    Uint MisClNo { 1 };
    auto it = find_if(Cells.begin(), Cells.end(), [&] (TCell * Entry) {
        if (MisClNo != Entry->Digit.No) return true;
        ++MisClNo;
        return false;
    });
    auto MisCell = next(Osdk.Digits.begin(), MisClNo - 1);
    TBoxMtrx cMtrx{ nullptr};
    copy(srMtrx.begin(), srMtrx.end(), cMtrx.begin());
    experimental::optional<TLoc> fl = FindInsertionLocation(cMtrx);
    if (fl) {
        TLoc nl{ fl->x + Lct.x, fl->y + Lct.y};
        MisCell->InsertNewItem(nl);
    } 
}
/******************************************************************************/
void TRibbon::AddCount(TCell& Cell) {
    ++Counts[Cell.Digit.No - 1];
}
/*******************************************************************************/
auto TRibbon::CheckMissingCellinOtherLines(TCell& Cell) {    
    TLine * MisLine{ nullptr};
    for (auto& e : RvLines) {
        TLine Line(*e);
        auto itl = find_if(Line.Cells.begin(), Line.Cells.end(), [&](auto& li) {
            return li->Digit.No == Cell.Digit.No;        });
        if (itl == Line.Cells.end()) {
            if (MisLine == nullptr) MisLine = e;
            else return false;
        }
    }
    return true;
}
/*******************************************************************************/
void TRibbon::CheckMissingOtherCell(TCell& Cell) {
    if (Counts[Cell.Digit.No - 1] == tree) return;
    if (CheckMissingCellinOtherLines(Cell)) {
        for (auto& e : vpBoxes) {
            auto Box(*e);
            if (Box.tn == Cell.ta || Box.Cells.size() < nine) continue;
            auto itt = find_if(Box.Cells.begin(), Box.Cells.end(), [&](auto& eb) {
                return eb->Digit.No == Cell.Digit.No; });
             if (itt == Box.Cells.end()) {
                Box.CRbn = lt;
                Box.Solution(Cell.Digit.No);
                if (Osdk.Cells.size() == eightyone || Counts[Cell.Digit.No - 1] == tree) return;
            }
      }
    } else CheckMissingCellinOtherBoxes(Cell);
    if (Cell.Digit.Count > 5) {
        if (Osdk.Cells.size() == eightyone) return;
        Cell.Digit.Solution();
    }
}
/*******************************************************************************/
void TRibbon::CheckMissingCellinOtherBoxes(TCell& Cell)   {
    for (auto& e : vpBoxes) {
        auto& Box(*e);
        if (Box.tn == Cell.ta) continue;
        auto itt = find_if(Box.Cells.begin(), Box.Cells.end(), [&](auto& ti) {
            return ti->Digit.No == Cell.Digit.No; });
        if (itt == Box.Cells.end()) {
            Box.CRbn = lt;
            Box.Solution(Cell.Digit.No);
            if (Counts[Cell.Digit.No - 1] == tree) return;
        }
    }
}
/*****************************************************************************/
void TLine::AddCell(TCell *cItm) {
    auto it = find_if(Cells.begin(), Cells.end(), [&] (TCell * Entry) {
        return Entry->Digit.No > cItm->Digit.No;    });
    Cells.insert(it, cItm);
}
/****************************************************************************/
auto MxLineItem = [&] (auto& l, auto& r) {
    if (l->Cells.size() == nine) return true;
    return l->Cells.size() < r->Cells.size() && r->Cells.size() < nine;
};
/*******************************************************************************/
void TSudoku::LineSolution(Uint n) {
    auto i = (n == 1) ? ltr : ltc;
    vector<TLine *> cLines;
    for(auto& e : Rows[i]) cLines.emplace_back(e.get());
    sort(cLines.begin(), cLines.end(), [&](const auto& l, const auto& r) {
            if (l->Cells.size() == r->Cells.size()) return l->No < r->No;
            return l->Cells.size() > r->Cells.size(); });
    for(auto& e : cLines) {
        auto sz { e->Cells.size() };
        if (e->Cells.size() > tree) {
            while (e->Cells.size() < nine) {
                e->Solution();
                auto nz = e->Cells.size();
                if ((nz - sz) == 0 || e->Cells.size() == nine) break;
                sz = nz;
            }
        }
        if (Inserted) return;
    }   
}
/****************************************************************************/
TLine::TLine(TSudoku *O, TRw *l, Uint n_, TLineType t) noexcept
      : Osdk { *O }, RwData{ *l}, No{n_}, lt{ t}    {
    auto m = (lt == ltr) ? No / tree * tree : No / tree;
    for (Uint i = 0; i < 3; i++) {
        LnBoxes.push_back(&Osdk.Boxes[m]);
        m += (lt == ltr) ? 1 : 3;
    }
}
/*******************************************************************************/
auto TLine::GenerateMisCellVect() {
    TvUint vMisCellNo;
    for (Uint i = 1; i < 10; ++i) {
        auto it = find_if(Cells.begin(), Cells.end(), [&] (const auto& e) { return e->Digit.No == i; } );
        if (it == Cells.end()) 
            vMisCellNo.emplace_back(i);
    }   
    if (vMisCellNo.size() + Cells.size() != nine) {
        cout << " vMisCellNo.size() + Cells.size() != dkz Line No ";
        PrintLine(RwData); 
        cout << " vMisCellNo  : size() " << vMisCellNo.size() << " vector "; for(auto& e : vMisCellNo) cout <<  e << " "; cout << endl;
        cout << " Line Cells vectoru : size () " << Cells.size() << " ";
        for(auto& e : Cells) cout << e->Digit.No << " "; cout << endl;
        PrintDoubleArray(" Exit 25", Osdk.Mtrx);
        exit(25);
    }
    return vMisCellNo;
}
/*******************************************************************************/
auto TLine:: GenerateMisLocVect()   {
   TvUint vMisLoclNo;
   Uint ml  { 0 };
   for(auto& el : RwData ) {
       if (el == nullptr) vMisLoclNo.emplace_back(ml);
       ++ml;
   }
   return vMisLoclNo;     
}
/*******************************************************************************/
bool TLine::InsertMissingCell(auto& MisCell) {
    auto il = RwData.begin();
    while (il != RwData.end() )  {
        il = find_if(il, RwData.end(), [&] (auto& e) { return e == nullptr; } );
        if (il == RwData.end()) return false;
        Uint i = (unsigned)  distance(RwData.begin(), il) / tree ;
        auto& t = LnBoxes[i];
        auto ip = find_if(t->Cells.begin(), t->Cells.end(), [&](auto& e) { return e->Digit.No == MisCell.No; } );
        if (ip == t->Cells.end())   {
            auto o = No - ((lt == ltr) ? LnBoxes[i]->Lct.x : LnBoxes[i]->Lct.y);
            auto& s = *(t->srMtrx.begin() + o);
            array<Uint*, 3u> sTrpLine = {nullptr};
            copy(s.begin(), s.end(), sTrpLine.begin());
            auto& rb = (lt == ltr) ? Osdk.Ribbons[1][t->tn % tree]
                     : Osdk.Ribbons[0][t->tn / tree];
            Uint j{ 0};
            Uint c{ 0}; // count
            Uint l{ 0}; // location
            for (auto& rl : rb->RvLines) {
                auto it = find_if(rl->Cells.begin(), rl->Cells.end(), [&](auto& e) {
                    return e->Digit.No == MisCell.No; });
                if (it != rl->Cells.end()) {
                    if (sTrpLine[j] == nullptr)
                        sTrpLine[j] = &MisCell.No;
                    ++c;
                } else {
                    l = j;
                }
                ++j;
            }
            if (c == two) {
                TLoc nl(lt == ltr ? No : LnBoxes[i]->Lct.x + l, lt == ltr ? LnBoxes[i]->Lct.y + l : No);
                MisCell.InsertNewItem(nl);
                return true;
            }
        } 
        ++il;
    }
 }
/*******************************************************************************/
void TDigit::InsertFoundLineLocation(const Uint& Ln, const TLineType& lt, TRw& cRow) {
     auto it = find_if(cRow.begin(), cRow.end(), [&](const auto& er) {
        return er == nullptr; });
    if (it != cRow.end()) {
        Uint d = (unsigned) distance(cRow.begin(), it);
        TLoc nl(lt == ltr ? Ln : d, lt == ltr ? d : Ln);
        InsertNewItem(nl);
    }
}

/*******************************************************************************/
bool TLine::TrytoInsertMisingCell(const Uint e, const TvUint& vMisCellNo, const bool v, const TRw& mv) {
    auto& MisCell = Osdk.Digits[e - 1]; 
    TRw tstV { (lt == ltr ? MisCell.tstV : MisCell.tstH) }; 
    if (v)  {
        auto it { tstV.begin() };
        for_each(mv.begin(), mv.end(), [&](auto& em) { 
            if (em != nullptr) *it = em;
            ++it;
        });
    }
    auto  it  { RwData.begin() }; 
    for_each(tstV.begin(), tstV.end(), [&](auto& et) { 
         if  ( et == nullptr ) et = *it;
         ++it;
    });
    bool found;
    Uint Lct;
    tie(found,Lct) = FillTstVectorLineTripleshasMissingCell(tstV,MisCell); 
    if (found == false) {
        auto vMisLocNo = GenerateMisLocVect();
        Uint j = (lt == ltr) ? 1 : 0;
        vector<TsLine>& r = Osdk.Rows[j];
        for(auto& ml : vMisLocNo) { 
            TsLine& re = *(next(r.begin(), ml));
            tie(found,Lct) = re->MissingOtherCellsContains(e, vMisCellNo);
            if (found) break;
        }
    }
    if (found) {
        TLoc nl(lt == ltr ? No : Lct, lt == ltr ? Lct : No);
        MisCell.InsertNewItem(nl);
        true;
    }
    return false;
}
/*******************************************************************************/
void TLine::Solution() {
    auto vMisCellNo = GenerateMisCellVect();
    TRw vTestModify { nullptr };
    auto rv = false; 
    while(vMisCellNo.size() > 0) {
        auto it = find_if(vMisCellNo.begin(), vMisCellNo.end(), [&](auto e) { 
            return this->TrytoInsertMisingCell(e, vMisCellNo, rv, vTestModify);
        } );
        if (it != vMisCellNo.end())
            vMisCellNo.erase(remove_if(vMisCellNo.begin(), vMisCellNo.end(), [&](const auto& e) { return e == *it; }), vMisCellNo.end());
        if (vMisCellNo.size() == 0 || it == vMisCellNo.end()) return;
    }
}
/*******************************************************************************/
PNewItem TLine::MissingOtherCellsContains(const Uint mce, const TvUint& vMisCellNo) {
    auto found { false };
    for(auto& e : vMisCellNo) {
        if (e == mce) continue;
        auto it = find_if(RwData.begin(), RwData.end(), [&](auto& le) 
            { return le != nullptr && *le == e; });
        if (it == RwData.end()) return make_pair( false,  No);
    }
    return make_pair( true,  No);
}
/*******************************************************************************/
void TLine::InsertFinalCell() {
    auto vMisCellNo = GenerateMisCellVect();
    auto vMisLocNo = GenerateMisLocVect();
    auto& MisCell = Osdk.Digits[vMisCellNo.front() - 1];   
    TLoc nl(lt == ltr ? No : vMisLocNo.front(), lt == ltr ? vMisLocNo.front() : No);
    MisCell.InsertNewItem(nl);
}
/*******************************************************************************/  
PNewItem TLine::FillTstVectorLineTripleshasMissingCell(TRw& tstV, TDigit& MisCell) {
   for (auto i = 0u; i < tree; ++i) {
        auto it = find_if(MisCell.Cells.begin(), MisCell.Cells.end(), [&](auto& ei) { 
            return ei->ta == LnBoxes[i]->tn;
        });
        if (it != MisCell.Cells.end()) {
            auto it = next( tstV.begin(), ( lt ==ltr ? LnBoxes[i]->Lct.y : LnBoxes[i]->Lct.x) );
            for(auto j = 0; j < 3; ++j, ++it) 
                if (*it == nullptr) 
                        *it = &MisCell.No;
        }
    }
    vector<Uint> vc;
    auto it = tstV.begin();
    find_if(tstV.begin(), tstV.end(), [&](auto& et) { 
        if (et == nullptr) { 
            vc.emplace_back( static_cast<Uint> (distance(tstV.begin(), it)));
            if (vc.size() > 1) return true;
        }
        ++it;
        return false;
    });
    return make_pair( vc.size() == 1,   vc.size() == 1 ? vc.front(): 0);
}
/*******************************************************************************/     
bool TDigit::InserttoOneofLocations(const Uint& Ln, const TLineType& lt, TRw& cRow, TvpBox& Box) {
    auto si = 0u;
    auto c = 0u;
    for (Uint i = 0; i < nine; ++i) {
        if (cRow[i] == nullptr) {
            Uint t = Box[i / 3]->tn;
            auto it = find_if(Cells.begin(), Cells.end(), [&](auto& e) {
                return e->ta == t;
            });
            if (it != Cells.end()) continue;  
            if (c == 1) return false;
            si = i;
            ++c;
        }
    }
    TLoc nl(lt == ltr ? Ln : si, lt == ltr ? si : Ln);
    InsertNewItem(nl);
    return true;
}
/*******************************************************************************/     
void TSudoku::RibbonSolution()  {
    for(auto& ei :Ribbons){
        for (auto& e : ei) {
            auto i = 0u;
            for_each(e->Counts.begin(), e->Counts.end(), [&] (const auto& ec) {
                if ( ec == two) 
                     e->CheckMissingThirdDigit(i);
                ++i;
            } );
        }
    }

}
/*******************************************************************************/   
 void TRibbon::CheckMissingThirdDigit(const Uint& n) {
     TDigit& Digit = *(next(Osdk.Digits.begin(), n ));
     auto it = find_if(RvLines.begin(), RvLines.end(), [&] (auto & el) {
         auto &Line { *el };
         auto itc = find_if(Line.Cells.begin(), Line.Cells.end(),  [&] (auto& ei) {
             return ei->Digit.No == Digit.No;
         } );
         return itc == Line.Cells.end();
    });
    TLine& Ln = **it;
    auto itb = find_if(vpBoxes.begin(), vpBoxes.end(), [&](auto& eb) {
        auto itc = find_if(eb->Cells.begin(), eb->Cells.end(), [&](auto& ei) {
            return ei->Digit.No == Digit.No;
        }); 
        return itc == eb->Cells.end();
    } );
    const auto& Loc = (*itb)->Lct;
    TRw cv { lt == ltr ? Digit.tstV : Digit.tstH };  
    auto itc { next(cv.begin(), lt == ltr ? Loc.y : Loc.x) };
    auto itl { next( Ln.RwData.begin(),  lt == ltr ? Loc.y : Loc.x) };
    auto c = 0u;
    auto fit = cv.end();
    for (auto i = 0; i < 3; ++i) {
        if (*itl != nullptr) 
           *itc = *itl;
        if (*itc != nullptr) 
            ++c;
        else if (fit == cv.end())
            fit = itc;
        else  return;
        ++itc;
        ++itl;
    }
    if (c == two) {
        auto d = static_cast<Uint> ( distance(cv.begin(), fit));
        TLoc nl { (lt == ltr ? Ln.No : d) ,  (lt == ltr ? d : Ln.No) };
        Digit.InsertNewItem(nl);
    }
}
 /******************************************************************************/
TPosibility :: TPosibility(TLine* er, TLine* ec, Uint& Prv) noexcept
             : Osdk{er->Osdk}, Row { *er }, Clm { *ec }
             , Next {Prv} 
             , Lct {Row.No, Clm.No} 
             , Box {Osdk.Boxes[(Lct.x / tree * tree) + Lct.y / tree] }
             , dVal {Osdk.Mtrx[Lct.x][Lct.y] }
             , cVal { Osdk.Mtrx[Lct.x][Lct.y] }
             , sPsbDigits({ 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u})
{
    ++Prv;                
    if (dVal == nullptr)
        GenerateSetofPosibility();
}
/******************************************************************************/
void TPosibility :: GenerateSetofPosibility() {
    for (auto i = 0u; i < 2; ++i ) {
        const TRw& RwData { i == 0 ? Row.RwData : Clm.RwData };
        for(const auto& erw : RwData ) {
            if (erw != nullptr) {
                auto it = find_if(sPsbDigits.begin(), sPsbDigits.end(),
                     [&](auto& e) { return e == *erw;  } );
                if (it != sPsbDigits.end())
                        sPsbDigits.erase(it);
            }
        }   
   }
   for (const auto& ebc : Box.Cells) {
       auto it = find_if(sPsbDigits.begin(), sPsbDigits.end(),
                     [&](auto& e) { return e == ebc->Digit.No;  } );
       if (it != sPsbDigits.end())
            sPsbDigits.erase(it);
   }
}
/******************************************************************************/
void WriteCellPosibilities(TItvsPosibility it, Uint Limit) {
    int i{ 0};
    cout << " Limit " << Limit << endl;
    while (i < Limit) {
        const TPosibility* Crnt((*it).get());
        if (i % 9 == 0) cout << endl;
        if (Crnt->dVal != nullptr) cout << '\t'<< Crnt->dVal ;
            else cout<< "\t" << Crnt->cVal ;
        ++it;
        ++i;
    }

}
/******************************************************************************/
bool TPosibility::BackTrace() {
    bool found = false;
    Uint Entry;
    if (dVal == nullptr) {
        auto its{sPsbDigits.begin()};
        while (its != sPsbDigits.end()) {
            tie(found, Entry) = CanbeInserted(its);
            if (found) {
                if (ContinueBackTrace(Entry))
                    return true;
            }
            if (its == sPsbDigits.end()) break;
            ++its;
        }
        return false;
    }
    if (Next > eighty) {
        Osdk.LastTrace = mdTerminate;
        return true;
    }
    if (Osdk.LastTrace == mdInsert) 
        return ContinueBackTrace(ten);
    return false;
}

/******************************************************************************/
bool TPosibility::ContinueBackTrace(Uint Entry){
    if (Entry < ten)
        InsertEntry(Entry);
    if (Next > 80) {
        Osdk.LastTrace = mdTerminate;
        return true;
    }
    auto it = next(Osdk.vPosible.begin(), Next);
    bool found { (*it)->BackTrace() };
    if (found == false) { 
        if (dVal == nullptr) {
            Row.UpdateRowDataDigit(Lct.y, nine);
            Clm.UpdateRowDataDigit(Lct.x, nine);
            TLoc bl{ Lct.x % tree, Lct.y % tree};
            Box.UpdateRowDataDigit(bl, nine);
            cVal = nullptr;
            Osdk.LastTrace = mdDelete;
        }
    } 
    return found;  
}
/******************************************************************************/
PNewItem TPosibility::CanbeInserted(TItvUint& its) {
    auto found = true;
    while (its != sPsbDigits.end()) {
        its = find_if(its, sPsbDigits.end(), [&](auto& Entry)   {
            if (Box.RowMatrixhasaDigit(Entry) ) 
                return true;
            if (Row.RowDatahasaDigit(Entry)) 
                return true;
            if (Clm.RowDatahasaDigit(Entry)) 
                return true;
            found = false;
            return  true;
        } );
        if (!found) break;
        ++its;
    }
    return make_pair(!found && its != sPsbDigits.end(), *its);

}
/******************************************************************************/
void TPosibility::InsertEntry(Uint Entry) {
    auto No{ Entry - 1};
    Row.UpdateRowDataDigit(Lct.y, No);
    Clm.UpdateRowDataDigit(Lct.x, No);
    TLoc bl{ Lct.x % tree, Lct.y % tree};
    Box.UpdateRowDataDigit(bl, No);
    cVal = &Osdk.Digits[No].No;  
    Osdk.LastTrace = mdInsert;
}