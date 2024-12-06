#include <stdio.h>

#include "Differentiator_func.h"

void CreateTex (Tree_t* tree)
{
    fprintf (tree->output, "\\documentclass[12pt,a4paper]{extreport}\n"
                            "\\usepackage[l2tabu,orthodox]{nag}\n"
                            "\\usepackage[left=15mm,right=15mm, top=20mm,bottom=20mm,bindingoffset=0cm]{geometry}\n"
                            "\\usepackage{indentfirst}\n"     
                            "\\usepackage[labelsep=period]{caption}\n"
                            "\\usepackage{amssymb,amsmath,amsthm}\n"
                            "\\usepackage{fontspec}\n"
                            "\\usepackage{float}\n"
                            "\\usepackage{array}\n"
                            "\\setmainfont[Ligatures=TeX]{STIX}\n"
                            "\\newfontfamily{\\cyrillicfont}[Ligatures=TeX]{STIX}\n"
                            "\\setmonofont{Fira Mono}\n"
                            "\\newfontfamily{\\cyrillicfonttt}{Fira Mono}\n"
                            "\\usepackage{polyglossia}\n"
                            "\\setdefaultlanguage{russian}\n"
                            "\\setotherlanguage{english}\n"
                            "\\usepackage{subcaption}\n"
                            "\\usepackage{graphicx}\n"
                            "\\graphicspath{img/}\n"
                            "\\DeclareGraphicsExtensions{.pdf,.png,.jpg}\n"
                            "\\usepackage{color}\n"
                            "\\definecolor{darkblue}{rgb}{0,0,.75}\n"
                            "\\definecolor{darkred}{rgb}{.7,0,0}\n"
                            "\\definecolor{darkgreen}{rgb}{0,.7,0}\n"
                            "\\usepackage[normalem]{ulem}\n"
                            "\\setlength{\\marginparwidth}{2cm}\n"
                            "\\usepackage[textwidth=4cm,textsize=tiny]{todonotes}\n"
                            "\\newcommand{\\fix}[2]{{\\textcolor{red}{\\uwave{#1}}\\todo[fancyline]{#2}}}\n"
                            "\\newcommand{\\hl}[1]{{\\textcolor{red}{#1}}}\n"
                            "\\newcommand{\\cmd}[1]{{\\ttfamily{\\textbackslash #1}}}\n"
                            "\\newcommand{\\vrb}[1]{\\PVerb{#1}}\n"
                            "\\newcommand{\\vrbb}[1]{\\texttt{\\textbackslash}\\PVerb{#1}}\n"
                            "\\usepackage[\n"
                            "draft = false,\n"
                            "unicode = true,\n"
                            "colorlinks = true,\n"
                            "allcolors = blue,\n"
                            "hyperfootnotes = true\n"
                            "]{hyperref}\n"
                            "\\usepackage{tikz}\n"
                            "\\usetikzlibrary{graphs, quotes}\n"
                            "\\usepackage{relsize}\n"
                            "\\usepackage{ytableau}\n"
                            "\\renewcommand \\thesection{\\Roman{section}}\n"
                            "\\renewcommand\\thesubsection{\\arabic{subsection}}\n"
                            "\\newcounter{homework}\n"
                            "\\newenvironment{homework}[1]%%\n"
                            "{\\refstepcounter{homework}%%\n"
                            "\\subsection{#1}}%%\n"
                            "{\\newpage}\n"
                            "\\newenvironment{homework*}[1]%%\n"
                            "{\\subsection*{#1}}%%\n"
                            "{\\newpage}\n"
                            "\\newcounter{task}\n"
                            "\\counterwithin{task}{homework}\n"
                            "\\newenvironment{task}[1]%%\n"
                            "{\\setcounter{task}{#1}\\addtocounter{task}{-1}\\refstepcounter{task}%%\n"
                            "\\par\\noindent\\textbf{Задача~\\thetask. }}%%\n"
                            "{\\smallskip}\n"
                            "\\newenvironment{solution}%%\n"
                            "{\\par\\noindent\\textbf{Решение. }}%%\n"
                            "{\\bigskip}\n"
                            "\\newcommand\\abs[1]{\\left\\lvert #1 \\right\\rvert}\n"
                            "\\newcommand\\ceil[1]{\\left\\lceil{#1}\\right\\rceil}\n"
                            "\\newcommand\\floor[1]{\\left\\lfloor{#1}\\right\\rfloor}\n"
                            "\\newcommand{\\divby}{\\;\\raisebox{-0.4ex}{\\vdots}\\;}\n"
                            "\\title{<<Дифференциатор от Бога>>}\n"       
                            "\\author{Быстрицкий Александр Б05-432}\n"
                            "\\begin{document}\n"
                            "\\maketitle\n"
                            "\\tableofcontents\n"
                            "\\begin{homework}{Берем производную}\n\\begin{task}{1}\n\t\\[f(x)="); 
    Print (tree->expression, tree->output); 
    fprintf (tree->output, ",f'(x)-?\\]\n\\end{task}\n\n\\begin{solution}\n");
}

void PutAnswerTex (Tree_t* tree)
{
    fprintf (tree->output, "\\text{После несложных подстановок получаем ответ:}\n");
    fprintf (tree->output, "\\["); 
    Print (tree->expression_diff, tree->output); 
    fprintf (tree->output, "\\]\n");
    fprintf (tree->output, "\\end{solution}\\end{homework}\\end{document}\n");
}