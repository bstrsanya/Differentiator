#include <stdio.h>
#include <stdlib.h>

#include "Differentiator_func.h"

void CreateTex (Tree_t* tree)
{
    fprintf (tree->output, "\\documentclass[12pt,a4paper]{article}\n"
                            "\\usepackage[left=15mm,right=15mm, top=20mm,bottom=20mm,bindingoffset=0cm]{geometry}\n"
                            "\\usepackage[utf8]{inputenc}\n"
                            "\\usepackage[T2A]{fontenc}\n"
                            "\\usepackage[russian]{babel}\n"
                            "\\usepackage{amsmath}\n"
                            "\\usepackage[\n"
                            "draft = false,\n"
                            "unicode = true,\n"
                            "colorlinks = true,\n"
                            "allcolors = blue,\n"
                            "hyperfootnotes = true\n"
                            "]{hyperref}\n"
                            "\\newenvironment{task}[1]%%\n"
                            "{\\par\\noindent\\textbf{Задача:}}%%\n"
                            "{\\smallskip}\n"
                            "\\newenvironment{solution}%%\n"
                            "{\\par\\noindent\\textbf{Решение. }}%%\n"
                            "{\\bigskip}\n"
                            "\\begin{document}\n"
                            "\\begin{titlepage}\n"
                            "\\vspace*{\\fill}\n"
                            "\\begin{center}\n"
                            "{\\LARGE\\bfseries <<Дифференциатор от Бога>>} \\\\[1cm]\n"
                            "{\\large Быстрицкий Александр Б05-432} \\\\[0.5cm]\n"
                            "{\\large \\today} \\\\[2cm] \n"
                            "\\end{center}\n"
                            "\\vspace*{\\fill}\n"
                            "\\end{titlepage}\n"
                            "\\begin{task}{1}\n\t\\[f(x)="); 
    Print (tree->expression, tree->output); 
    fprintf (tree->output, ", \\quad f'(x)-?\\]\n\\end{task}\n\n\\begin{solution}\n");
}

void PutAnswerTex (Tree_t* tree)
{
    fprintf (tree->output, "\\text{После несложных подстановок получаем ответ:}\n");
    fprintf (tree->output, "\\["); 
    Print (tree->expression_diff, tree->output); 
    fprintf (tree->output, "\\]\n");
    fprintf (tree->output, "\\end{solution}\\end{document}\n");
    fclose (tree->output);
    tree->output = NULL;
    system ("pdflatex --output-directory=" DIR " " INPUT " " OUTPUT ">/dev/null");
    // system ("cp ./output/dif.pdf dif.pdf");
}