
/* 
 * File:   Sudoku.h
 * Author: Sabetay Toros 10 August
 *
 * Created on  27 Sep 18.15
 */
#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>

using Uint = unsigned int;
constexpr Uint zero = 0u;
constexpr Uint one = 1u;
constexpr Uint two = 2u;
constexpr Uint tree = 3u;
constexpr Uint five = 5u;
constexpr Uint eight = 8u;
constexpr Uint nine = 9u;
constexpr Uint ten = 10u;
constexpr Uint eighty = 80u;
constexpr Uint eightyone = 81u;
using TRw =  array<Uint *, nine>;
using TDblMtrx = array<TRw, nine>;
using TBoxMtrx = array<array<Uint*, tree>, tree>;
struct TCell;
struct TDigit;
struct TBox;
struct TLine;
struct TRibbon;
struct TSudoku;
using TvUint = vector<Uint>;
using Tsfstream = shared_ptr<fstream>;
/******************************************************************************/
struct TSdkData {
    string Identity;
    bool intr = false;
    TvUint Data; 
    bool State = false;
    TSdkData(string s, fstream& fs);
};
/******************************************************************************/
struct TLoc {
    const Uint x;
    const Uint y;
    TLoc(Uint i, Uint j) : x { i }, y { j } {};
    ~TLoc() {  } // cout <<  " Loc destroy edildi "  << endl;
    TLoc operator+(const TLoc& l) { return TLoc(x + l.x, y + l.y); }
    TLoc operator-(const TLoc& l) { return TLoc(x - l.x, y - l.y); }
    friend ostream& operator<<(ostream& os, const TLoc& v);
};
using TvLoc = vector<TLoc>;
/******************************************************************************/
struct TCell {
    TDigit &Digit;
    const TLoc Lct;
    const Uint ta; // Triple No
    const Uint tr;
    const Uint tc;
    bool isData = false;
    TCell(TDigit* o, TLoc lct, bool d);
        friend ostream& operator<<(ostream& os, const TCell& It); 
};
using TsCell = shared_ptr<TCell>;
using TvCell = vector<TCell *>;
enum TLineType { ltr, ltc } ;
struct TBox;
using TvBox = vector<TBox>;
using TvpBox = vector<TBox *>;
/******************************************************************************/
struct TDigit {
                 TDigit(TSudoku * o, auto No_); 
            bool operator<(const TDigit& c) const {
                return  Count < c.Count && c.Count < 9;
            }
 friend ostream& operator<<(ostream& os, const TDigit& v);
            void GenerateItem(TLoc& lc, bool Dflt);
            void Solution();
            void FindMissingRow(auto& Row, Uint&  mr);// 
            void InsertFinalCell();
            void FitBlankCells();
            auto FillTestRowWithNonCellValues(auto im, auto& tstV, Uint mr, TLineType t);
            bool InsertNewItem(TLoc& lc );
            void InsertFoundLineLocation(const Uint& Ln, const TLineType& lt, TRw& cRow);
            bool InserttoOneofLocations(const Uint& Ln, const TLineType& lt, TRw& cRow, TvpBox& Box);

         TSudoku &Osdk;
            Uint &No;
vector <TsCell > Cells;
            Uint Count { 0 };
             TRw tstH = { nullptr };
             TRw tstV = { nullptr };                
};
using TvDigits = vector<TDigit>;
/*****************************************************************T*************/
struct TBox {
                 TBox(TSudoku *o, Uint  tn_) noexcept; 
 friend ostream& operator<<(ostream& os, const TBox& v);
            void AddCell(TCell& Cell) ;
            void Solution(); 
            auto Solution(Uint MsClNo);
            auto GenerateChechSubMatrix(TDigit& Digit);
experimental::optional<TLoc> FindInsertionLocation(TBoxMtrx& cMtrx);
            void InsertFinalCell();
            void InsertOtherCells(); 
            bool RowMatrixhasaDigit(const Uint Entry);
            void UpdateRowDataDigit(const TLoc& l, const Uint No);
            
         TSudoku &Osdk;
            Uint tn;                    // Triple No
      const TLoc Lct;                     // Absolute Location 
      const TLoc Rbn;
       TLineType CRbn = ltr;
        TBoxMtrx srMtrx { nullptr };   //  Row Box Sub Matrix
        TBoxMtrx scMtrx { nullptr };   //  Clmn Box Sub Matrix
          TvCell Cells;    //  
};
using TvBox = vector<TBox>;
using TvpBox = vector<TBox *>;
using PNewItem = pair <bool, Uint>;
/******************************************************************************/
struct TLine {   
                 ~TLine() { }                 
                 TLine(TSudoku *O, TRw *l, Uint  n_, TLineType t) noexcept; 
 friend ostream& operator<<(ostream& os, const TLine& v);
            void AddCell(TCell *Item);
            void Solution();
            auto GenerateMisCellVect();
            auto GenerateMisLocVect();
            bool InsertMissingCell(auto& MisCell);
            void InsertFinalCell();
            void CheckMissingCell(TvUint& vMsCelNo, TvUint& vMsCelLc);
        PNewItem MissingOtherCellsContains(const Uint e, const TvUint& vMisCellNo);
        PNewItem FillTstVectorLineTripleshasMissingCell(TRw& tstV,TDigit& MisCell);
            bool TrytoInsertMisingCell(const Uint e, const TvUint& vMisCellNo, const bool v, const TRw& mv);
            bool RowDatahasaDigit(const Uint Entry);   
            void UpdateRowDataDigit(const Uint i, const Uint No);     // index, Digit No 
            
         TSudoku &Osdk;          // Owner
             TRw &RwData;        // Column or Row Data;                            
      const Uint No;             // Column Row No
 const TLineType lt;
          TvpBox LnBoxes;        // Triples 3 pcs
          TvCell Cells;          // 
};
using TsLine = shared_ptr<TLine>;
using TvsLine = vector<vector<TsLine>>;
using TvpLine = vector<TLine *>;
using TCount = array<Uint, nine>;
using FoundP = pair<TLine *, TCell *>;

/******************************************************************************/
using TvRow = vector<TRw>;
struct TRibbon {
                TRibbon(TSudoku *O, Uint n,  TLineType t) noexcept ;
                ~TRibbon() {  } 
 friend ostream& operator<<(ostream& os, const TRibbon& v);    
            void AddCount(TCell& Cell);
            void CheckMissingOtherCell(TCell& Cell); 
            auto CheckMissingCellinOtherLines(TCell& Cell);
            void CheckMissingCellinOtherBoxes(TCell& Cell);
            void CheckMissingThirdDigit(const Uint& d);

         TSudoku &Osdk;             // Owner
           Uint  No;                // Ribbon No
 const TLineType lt;                // Horizontal, Vertical
         TvpLine RvLines;           // Lines
          TvpBox vpBoxes;            // Triples
          TCount Counts { 0 };      // Cell Counts in Ribbon Max 3
            Uint ctn;               // current triple no when Checked

};
struct TPosibility;
using TvsPosibility = vector<shared_ptr<TPosibility> >;
using TItvsPosibility =  TvsPosibility::iterator; 
using TItvUint = TvUint::iterator;

/******************************************************************************/
struct TPosibility {

                    TPosibility(TLine* re, TLine* ce, Uint& Prv) noexcept;
               void GenerateSetofPosibility();
               bool BackTrace();
           PNewItem CanbeInserted(TItvUint& its);
               bool ContinueBackTrace(Uint Entry);
               void InsertEntry(Uint Entry);
    friend ostream& operator<<(ostream& os, const TPosibility& v);    
              
           TSudoku& Osdk;                  // Owner 
             TLine& Row;                   // Row
             TLine& Clm;                   // Column 
               Uint Next;                 // Next this is obselete
         const TLoc Lct;                   // Location
              TBox& Box;                   // Box 
        const Uint* dVal;                   // Assigned Default Digit 
              Uint* cVal;                   // Current value
             TvUint sPsbDigits;            // Set of Posibble Digits 
};
using TCounts = array<TCount, 4>;
enum mdTrace { mdInsert, mdDelete, mdTerminate };

/******************************************************************************/
using TvsRibbon = vector<vector<shared_ptr<TRibbon> > >;
struct TSudoku {
                ~TSudoku() {};
                 TSudoku(TSdkData& d) noexcept;
            void SelectaSolution();    
            void DigitSolution(); 
            void LineSolution(Uint n);
            void RibbonSolution();
            void BackTraceSolution();

        TDblMtrx Mtrx{ nullptr};        // Sudoku Row Matrix
        TDblMtrx Clmn{nullptr};         // Sudoku Column Matrix
         TCounts Counts{0};     
        TvDigits Digits;                //{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
           TvBox Boxes;                 //{ 0, 1, 2, 3, 4, 5, 6, 7, 8};
         TvsLine Rows;
       TvsRibbon Ribbons;
          TvCell Cells;
   TvsPosibility vPosible;
            bool Inserted;
            bool ChckR = false;
         mdTrace LastTrace = mdInsert;
};
/******************************************************************************/

#endif /* SUDOKU_H */