
#include <iostream>
#include <phpcpp.h>
#include <PDFWriter/PDFWriter.h>
#include <PDFWriter/PDFModifiedPage.h>
#include <PDFWriter/PDFPage.h>
#include <PDFWriter/PDFObject.h>
#include <PDFWriter/AbstractContentContext.h>

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
        Php::out << "InputFile" << params[0] << std::endl;
        Php::out << "OutputFile" << params[1] << std::endl;

        writer.ModifyPDF(params[0], ePDFVersion14, params[1]);
    }

    void writeTextToPage(Php::Parameters &params)
    {
        Php::out << "x: " << params[0] << std::endl;
        Php::out << "y: " << params[1] << std::endl;
        Php::out << "Text: " << params[2] << std::endl;

        PDFModifiedPage thePage(&writer,0,true);
        AbstractContentContext* contentContext = thePage.StartContentContext();

        PDFUsedFont * font = writer.GetFontForFile("./arial.ttf",0);
        if(!font) {
            Php::out << "Failed to retrieve font ./arial.ttf" << std::endl;
            return;
        }
        AbstractContentContext::TextOptions opt(font,12,AbstractContentContext::eRGB, 0);
        contentContext->WriteText((double)params[0], (double)params[1], params[2], opt);
        contentContext->WriteText((double)params[0], (double)params[1]-100, params[2], opt);
        contentContext->WriteText((double)params[0], (double)params[1]-200, params[2], opt);
        thePage.EndContentContext();
        thePage.WritePage();
        Php::out << "HERE: " << __LINE__ << std::endl;
/*

        if(params[0] != pageNum) {
            Php::out << "HERE: " << __LINE__ << std::endl;

            if (modifiedPage != NULL){
                modifiedPage->EndContentContext();
                modifiedPage->WritePage();
                Php::out << "HERE: " << __LINE__ << std::endl;
                delete modifiedPage;
            }
            Php::out << "HERE: " << __LINE__ << std::endl;

            PDFModifiedPage thePage(&writer,(int)params[0],true);
            modifiedPage = &thePage;

            Php::out << "HERE: " << __LINE__ << std::endl;
            if (contentContext != NULL) {
                delete contentContext;
            }

            Php::out << "HERE: " << __LINE__ << std::endl;
            contentContext = modifiedPage->StartContentContext();

            pageNum = params[0];
        }
        Php::out << "HERE: " << __LINE__ << std::endl;

        AbstractContentContext::TextOptions opt(writer.GetFontForFile("/home/gnat/Projects/pdf-writer-test/fonts/arial.ttf",0),12,AbstractContentContext::eRGB, 0);
        Php::out << "HERE: " << __LINE__ << std::endl;
        contentContext->WriteText(params[1],params[2],params[3],opt);
        Php::out << "HERE: " << __LINE__ << std::endl;
        */
    }

    void writePdf()
    {
//        if(modifiedPage != NULL) {
//            modifiedPage->EndContentContext();
//            modifiedPage->WritePage();
//            delete modifiedPage;
//        }

        writer.EndPDF();
        Php::out << "HERE: " << __LINE__ << std::endl;

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
        
        Php::Class<PdfWriter> pdfWriter("PdfWriter");
        pdfWriter.method<&PdfWriter::__construct>("__construct", {
                    Php::ByVal("inputFile", Php::Type::String),
                    Php::ByVal("outputFile", Php::Type::String)
                    });

        pdfWriter.method<&PdfWriter::writeTextToPage>("writeTextToPage",{
                Php::ByVal("x",Php::Type::Numeric),
                Php::ByVal("y",Php::Type::Numeric),
                Php::ByVal("text",Php::Type::String),
        });


        pdfWriter.method<&PdfWriter::writePdf>("save");
        extension.add(pdfWriter);

        // return the extension
        return extension;
    }
}
