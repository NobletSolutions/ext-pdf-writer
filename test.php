<?php

if (class_exists('\PdfWriter')) {
    $pdfWriter = new \PdfWriter('./File.pdf','./Output.pdf');

    echo "Class exists!\n";
	$pdfWriter->writeTextToPage(78.5,640,"The text to write");/*
	$pdfWriter->writeTextToPage(78.5,540,"More text to write");
	$pdfWriter->writeTextToPage(78.5,440,"Gnat defaced this PDF!");*/
    $pdfWriter->save();

} else {
    echo "Class doesn't exist\n";
}

