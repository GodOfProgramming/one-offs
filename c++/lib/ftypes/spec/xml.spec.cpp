#include <spec.hpp>
#include "xml.h"

BeginSpec(XML);
Describe("class XML", [] {
    Context("parse()", [] {
        Context("invalid format", [] {
            const char* valid_xml =
                "<?xml version=\"1.0\" encoding=\"utf8\"?>"
                "<first>"
                "<second>"
                "</second>"
                "</first";  // unclosed tag
            It("returns false", [&] {
                xml::XML doc;
                Expect(doc.parse(valid_xml)).toEqual(false);
            });
        });
        Context("valid format", [] {
            const char* valid_xml =
                "<?xml version=\"1.0\" encoding=\"utf8\"?>"
                "<first>"
                "<second>"
                "</second>"
                "</first>";
            It("returns true", [&] {
                xml::XML doc;
                Expect(doc.parse(valid_xml)).toEqual(true);
            });
        });
    });
});
EndSpec();
