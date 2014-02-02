/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_12958715_INCLUDED
#define BINARYDATA_H_12958715_INCLUDED

namespace BinaryData
{
    extern const char*   default_values_xml;
    const int            default_values_xmlSize = 1377;

    extern const char*   default_exporters_xml;
    const int            default_exporters_xmlSize = 629;

    extern const char*   prefs_export_png;
    const int            prefs_export_pngSize = 3839;

    extern const char*   mdl_file_header_txt;
    const int            mdl_file_header_txtSize = 2266;

    extern const char*   about_txt;
    const int            about_txtSize = 407;

    extern const char*   prefs_about_png;
    const int            prefs_about_pngSize = 1819;

    extern const char*   prefs_misc_png;
    const int            prefs_misc_pngSize = 6162;

    extern const char*   icons_zip;
    const int            icons_zipSize = 12375;

    extern const char*   synthamodeler_icon_png;
    const int            synthamodeler_icon_pngSize = 25836;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 9;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
