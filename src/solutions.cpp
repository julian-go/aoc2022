#include "solutions.h"

#include "y22.h"

const Problems problems = {Year{
    true, "2022", "Y22",
    std::array{
        Problem{1, y22day01, {"24000", "45000"}, {"71300", "209691"}},
        Problem{1, y22day02, {"15", "12"}, {"12772", "11618"}},
        Problem{1, y22day03, {"157", "70"}, {"8298", "2708"}},
        Problem{1, y22day04, {"2", "4"}, {"584", "933"}},
        Problem{1, y22day05, {"CMZ", "MCD"}, {"TWSGQHNHL", "JNRSCDWPP"}},
        Problem{1, y22day06, {"7", "19"}, {"1723", "3708"}},
        Problem{1, y22day07, {"95437", "24933642"}, {"2061777", "4473403"}},
        Problem{1, y22day08, {"21", "8"}, {"1543", "595080"}},
        Problem{1, y22day09, {"88", "36"}, {"5874", "2467"}},
        Problem{1, y22day10, {"13140", "3470160795462901799"}, {"13920", "10069953020766180745"}},
        Problem{1, y22day11, {"10605", "2713310158"}, {"67830", "15305381442"}},
        Problem{1, y22day12, {"31", "29"}, {"456", "454"}},
        Problem{1, y22day13, {"13", "140"}, {"6101", "21909"}},
        Problem{1, y22day14, {"24", "93"}, {"1001", "27976"}},
        Problem{1, y22day15, {"26", "56000011"}, {"4560025", "12480406634249"}},
        Problem{1, y22day16, {"1651", "1707"}, {"1850", "0"}},
        Problem{0, y22day17, {"3068", "1514285714288"}, {"3065", "1562536022966"}},
        Problem{1, y22day18, {"64", "58"}, {"4460", "2498"}},
        Problem{0, y22day19, {"33", "62"}, {"1719", "19530"}},
        Problem{1, y22day20, {"3", "1623178306"}, {"7395", "N/A"}},
        Problem{1, y22day21, {"152", "301"}, {"24947355373338", "3876907167495"}},
        Problem{1, y22day22, {"6032", "5031"}, {"117102", "N/A"}},
        Problem{1, y22day23, {"110", "20"}, {"4000", "1040"}},
        Problem{1, y22day24, {"18", "54"}, {"257", "828"}},
        Problem{1, y22day25, {"2=-1=0", "N/A"}, {"2=001=-2=--0212-22-2", "N/A"}}}}};

const Problems& getProblems() { return problems; }
