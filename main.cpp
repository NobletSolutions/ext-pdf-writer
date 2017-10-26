
#include <iostream>
#include <phpcpp.h>
#include <PDFWriter/PDFWriter.h>
#include <PDFWriter/PDFModifiedPage.h>
#include <PDFWriter/PDFPage.h>
#include <PDFWriter/PDFObject.h>
#include <PDFWriter/AbstractContentContext.h>

class PdfText : public Php::Base {
private:
    Php::Value x;
    Php::Value y;
    Php::Value text;
public:
    PdfText() = default;

    void __construct(Php::Parameters &params)
    {
//        Php::out << "x: " << params[0] << std::endl;
//        Php::out << "y: " << params[1] << std::endl;
//        Php::out << "Text: " << params[2] << std::endl;
        x = params[0];
        y = params[1];
        text = params[2];
    }

    Php::Value getX() {
        return x;
    }

    Php::Value getY() {
        return y;
    }

    Php::Value getText() {
        return text;
    }
};

class PdfWriter : public Php::Base {
private:
    PDFWriter writer;
    PDFModifiedPage * modifiedPage = NULL;
    PDFObject * page = NULL;
    AbstractContentContext * contentContext = NULL;
    AbstractContentContext::TextOptions * textOptions = NULL;

    int64_t pageNum = -1;
public:
    PdfWriter() = default;

    void __construct(Php::Parameters &params)
    {
//        Php::out << "InputFile" << params[0] << std::endl;
//        Php::out << "OutputFile" << params[1] << std::endl;

        writer.ModifyPDF(params[0], ePDFVersion14, params[1]);
    }

    void writeTextToPage(Php::Parameters &params)
    {
        if (params[0] < 0) {
//            Php::out << "Page Is Negative!" << std::endl;
            return;
        }

//        Php::out << "Page: " << params[0] << std::endl;
//        Php::out << "Count: " << params[1].size() << std::endl;
//        for (auto &iter : params[1]) {
//            PdfText *obj = (PdfText *)iter.second.implementation();
//
//            Php::out << "Loop: " << iter.first << std::endl;
//            Php::out << "   x: " << obj->getX() << std::endl;
//            Php::out << "   y: " << obj->getY() << std::endl;
//            Php::out << "   t: " << obj->getText() << std::endl;
//        }

        if (params[1].size() > 0) {
            PDFModifiedPage thePage(&writer, static_cast<double>(params[0]), true);
            AbstractContentContext* contentContext = thePage.StartContentContext();

            PDFUsedFont * font = writer.GetFontForFile("./arial.ttf",0);
            if (!font) {
//                Php::out << "Failed to retrieve font ./arial.ttf" << std::endl;
                return;
            }

            AbstractContentContext::TextOptions opt(font,12,AbstractContentContext::eRGB, 0);
            for (auto &iter : params[1]) {
                PdfText *obj = (PdfText *)iter.second.implementation();
                contentContext->WriteText((double)obj->getX(), (double)obj->getY(), obj->getText(), opt);
            }

            thePage.EndContentContext();
            thePage.WritePage();
        }

        return;
    }

    void writePdf()
    {
        writer.EndPDF();
    }
};

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("pdf-writer", "0.1");

        Php::Namespace myNamespace("PDF");

        Php::Class<PdfText> pdfText("PdfText");
        pdfText.method<&PdfText::__construct>("__construct", {
                    Php::ByVal("x", Php::Type::Numeric),
                    Php::ByVal("y", Php::Type::Numeric),
                    Php::ByVal("text", Php::Type::String)
                    });
        pdfText.method<&PdfText::getX>("getX");
        pdfText.method<&PdfText::getY>("getY");
        pdfText.method<&PdfText::getText>("getText");
        pdfText.method<&PdfText::getText>("__toString");

        Php::Class<PdfWriter> pdfWriter("PdfWriter");
        pdfWriter.method<&PdfWriter::__construct>("__construct", {
                    Php::ByVal("inputFile", Php::Type::String),
                    Php::ByVal("outputFile", Php::Type::String)
                    });

        pdfWriter.method<&PdfWriter::writeTextToPage>("writeTextToPage",{
                Php::ByVal("page",Php::Type::Numeric),
                Php::ByVal("modifications",Php::Type::Array)
        });


        pdfWriter.method<&PdfWriter::writePdf>("save");

        myNamespace.add(pdfText);
        myNamespace.add(pdfWriter);
        extension.add(myNamespace);
//        extension.add(pdfText);
//        extension.add(pdfWriter);

        // return the extension
        return extension;
    }
}
