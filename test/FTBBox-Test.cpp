#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTBBox.h"

static const int RESOLUTION = 72;
static const int CHARACTER_INDEX = 'g';
static const int CHARACTER_SIZE = 72 * 64;
static const char* FONT_FILE = "../../test/MHei-Medium-Acro";

class FTBBoxTest : public CppUnit::TestCase
{
        CPPUNIT_TEST_SUITE( FTBBoxTest);
            CPPUNIT_TEST( testDefaultConstructor);
            CPPUNIT_TEST( testGlyphConstructor);
            CPPUNIT_TEST( testMoveBBox);
//            CPPUNIT_TEST( testAddBBox);
            CPPUNIT_TEST( testPlusEquals);
        CPPUNIT_TEST_SUITE_END();
        
    public:
        FTBBoxTest() : CppUnit::TestCase( "FTBBox Test")
        {}
        
        FTBBoxTest( const std::string& name) : CppUnit::TestCase(name) {}

        void testDefaultConstructor()
        {
            FTBBox boundingBox1;

            CPPUNIT_ASSERT( boundingBox1.lowerX == 0.0f);
            CPPUNIT_ASSERT( boundingBox1.lowerY == 0.0f);
            CPPUNIT_ASSERT( boundingBox1.lowerZ == 0.0f);
            CPPUNIT_ASSERT( boundingBox1.upperX == 0.0f);
            CPPUNIT_ASSERT( boundingBox1.upperY == 0.0f);
            CPPUNIT_ASSERT( boundingBox1.upperZ == 0.0f);
        }
        
        
        void testGlyphConstructor()
        {    
            setUpFreetype();

            FTBBox boundingBox2( glyph);

            CPPUNIT_ASSERT_DOUBLES_EQUAL(   3, boundingBox2.lowerX, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL( -15, boundingBox2.lowerY, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   0, boundingBox2.lowerZ, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  35, boundingBox2.upperX, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  38, boundingBox2.upperY, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   0, boundingBox2.upperZ, 0.01);

            tearDownFreetype();
        }     

        void testMoveBBox()
        {
            FTBBox  boundingBox;
            FTPoint firstMove( 3.5f, 1.0f, -2.5f);
            FTPoint secondtMove( -3.5f, -1.0f, 2.5f);
            
            boundingBox.Move( firstMove);
        
            CPPUNIT_ASSERT( boundingBox.lowerX ==  3.5f);
            CPPUNIT_ASSERT( boundingBox.lowerY ==  1.0f);
            CPPUNIT_ASSERT( boundingBox.lowerZ == -2.5f);
            CPPUNIT_ASSERT( boundingBox.upperX ==  3.5f);
            CPPUNIT_ASSERT( boundingBox.upperY ==  1.0f);
            CPPUNIT_ASSERT( boundingBox.upperZ == -2.5f);

            boundingBox.Move( secondtMove);
        
            CPPUNIT_ASSERT( boundingBox.lowerX == 0.0f);
            CPPUNIT_ASSERT( boundingBox.lowerY == 0.0f);
            CPPUNIT_ASSERT( boundingBox.lowerZ == 0.0f);
            CPPUNIT_ASSERT( boundingBox.upperX == 0.0f);
            CPPUNIT_ASSERT( boundingBox.upperY == 0.0f);
            CPPUNIT_ASSERT( boundingBox.upperZ == 0.0f);
        }
        
        void testPlusEquals()
        {
            setUpFreetype();

            FTBBox boundingBox1;
            FTBBox boundingBox2( glyph);
            
            boundingBox1 += boundingBox2;
        
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   3, boundingBox2.lowerX, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL( -15, boundingBox2.lowerY, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   0, boundingBox2.lowerZ, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  35, boundingBox2.upperX, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  38, boundingBox2.upperY, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   0, boundingBox2.upperZ, 0.01);
            
            float advance  = 40;
            
            boundingBox2.Move( FTPoint( advance, 0, 0));
            boundingBox1 += boundingBox2;
            
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  43, boundingBox2.lowerX, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL( -15, boundingBox2.lowerY, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   0, boundingBox2.lowerZ, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  75, boundingBox2.upperX, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(  38, boundingBox2.upperY, 0.01);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(   0, boundingBox2.upperZ, 0.01);

            tearDownFreetype();
        }
        
        void setUp() 
        {}
        
        
        void tearDown() 
        {}
        
    private:
        FT_Library   library;
        FT_Face      face;
        FT_Glyph     glyph;

        void setUpFreetype()
        {
            FT_Error error = FT_Init_FreeType( &library);
            assert(!error);
            error = FT_New_Face( library, FONT_FILE, 0, &face);
            assert(!error);
            
            long glyphIndex = FT_Get_Char_Index( face, CHARACTER_INDEX);
            
            FT_Set_Char_Size( face, 0L, CHARACTER_SIZE, RESOLUTION, RESOLUTION);
            
            error = FT_Load_Glyph( face, glyphIndex, FT_LOAD_DEFAULT);
            assert(!error);
            error = FT_Get_Glyph( face->glyph, &glyph);
            assert(!error);
        }
        
        void tearDownFreetype()
        {
            FT_Done_Glyph( glyph);
            FT_Done_Face( face);
            FT_Done_FreeType( library);
        }
        
};

CPPUNIT_TEST_SUITE_REGISTRATION( FTBBoxTest);
