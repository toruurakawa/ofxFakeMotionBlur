//
//  ofxFakeMotionBlur.h
//
//  Created by Toru Urakawa on 12/05/18.
//

#ifndef _ofxFakeMotionBlur
#define _ofxFakeMotionBlur

#include "ofMain.h"
#include <gl/glut.h>

#define TEXNUM 8
#define TEXWIDTH ofGetWidth()
#define TEXHEIGHT ofGetHeight()

class ofxFakeMotionBlur{
public:
    void setup(){
        
        stringstream srcVert;
        srcVert << "#version 120\n";
        srcVert << "void main(void) {\n";
        srcVert << "gl_Position = ftransform();//gl_ModelViewProjectionMatrix * gl_Vertex;\n";
        srcVert << "gl_TexCoord[0] = gl_MultiTexCoord0;\n";
        srcVert << "}\n";

        stringstream srcFrag;
        srcFrag << "#version 120\n";
        for(int i=0; i<TEXNUM; i++){
            srcFrag << "uniform sampler2D sampler"<< i <<";\n";
        }
        srcFrag << "void main(void) {\n";
        for(int i=0; i<TEXNUM; i++){
            srcFrag << "vec4 texColor"<< i << " = texture2D(sampler"<< i << ", gl_TexCoord[0].st);\n";
        }        
        srcFrag << "vec4 resColor;\n";
        for(int i=0; i<TEXNUM; i++){
            srcFrag << "resColor += texColor"<<i<<";\n";
        }
        srcFrag << "resColor /= "<<TEXNUM<<".;\n";
        srcFrag << "resColor.a = 1.0;";
        srcFrag << "gl_FragColor = resColor;";
        srcFrag << "}\n";
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, srcVert.str());
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, srcFrag.str());
        shader.linkProgram();
        doShader = 1;	
        
        for(int i=0; i<TEXNUM; i++){
            char sampler[128];
            sprintf(sampler, "sampler%d", i);
            shader.setUniform1i(sampler, i); 
        }

        setTexture();
        initFramebuffer();
    }
    
    void update(){
        count = (count+1)%3;
        if(count==0){
            indicator = (indicator+1)%TEXNUM;
        }
    };
    
    void draw(){
        if(doShader)
            shader.begin();
        
        for(int i=0; i<TEXNUM; i++){
            char sampler[128];
            sprintf(sampler, "sampler%d", i);
            shader.setUniform1i(sampler, i); 
        }
        
        ofPushMatrix();
        drawGLObject();
        ofPopMatrix();
        
        if(doShader)
            shader.end();
    }
    
    void begin(){
        this->beginRenderingFbo();
    }
    
    void end(){
        this->endRenderingFbo();
    }
    
protected:
    
    GLuint rbNames[TEXNUM];
    GLuint fbNames[TEXNUM];
    GLuint texNames[TEXNUM];
    
    void initFramebuffer(){   
        
        for(int i=0; i<TEXNUM; i++){
            //  generate renderbuffer object
            glGenRenderbuffersEXT( 1, &(rbNames[i]) );
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rbNames[i] );
            //  Z
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXWIDTH, TEXHEIGHT );
            //  generate framebuffer object
            glGenFramebuffersEXT( 1,  &(fbNames[i]) );
            glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbNames[i] );
            //  connect texture to framebuffer object
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                      GL_TEXTURE_2D, texNames[i], 0 );
            
            //  connect renderbuffer to framebuffer object
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                         GL_RENDERBUFFER_EXT, rbNames[i] );
            
            glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
        }    
        
    }
    
    void setTexture(){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (GLuint i=0; i<TEXNUM; i++) {
            // generate texture objects for each texture unit
            glGenTextures(1, &(texNames[i])); 
            // switch to  texture unit "i"
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, texNames[i]);  
            // set texture
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,GL_RGBA,GL_UNSIGNED_BYTE,0);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        glActiveTexture(GL_TEXTURE0);
    }
    
    void drawGLObject(){
        static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };  
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        
        // activate texture units
        for (GLuint i=0; i<TEXNUM; i++) {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, texNames[i]);
            glEnable(GL_TEXTURE_2D);    
        }

        glNormal3d(0.0, 0.0, 1.0);
        glBegin(GL_QUADS);
        for (GLuint i=0; i<TEXNUM; i++) {
            glMultiTexCoord2d(GL_TEXTURE0+i, 0.0, 1.0);
        }
        glVertex3d(0, 0,  0.0);
        for (GLuint i=0; i<TEXNUM; i++) {
            glMultiTexCoord2d(GL_TEXTURE0+i, 1.0, 1.0);
        }
        glVertex3d(TEXWIDTH, 0,  0.0);
        for (GLuint i=0; i<TEXNUM; i++) {
            glMultiTexCoord2d(GL_TEXTURE0+i, 1.0, 0.0);
        }
        glVertex3d( TEXWIDTH, TEXHEIGHT,  0.0);
        for (GLuint i=0; i<TEXNUM; i++) {
            glMultiTexCoord2d(GL_TEXTURE0+i, 0.0, 0.0);
        }
        glVertex3d(0,  TEXHEIGHT,  0.0);
        glEnd();
        
        // finish texture mapping
        for (int i=TEXNUM-1; i>=0; i--) {
            glActiveTexture(GL_TEXTURE0+i);
            glDisable(GL_TEXTURE_2D);
        }
        
    }
    
    void beginRenderingFbo(){
        //  begin rendering to FBO 
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbNames[indicator]);
        //  set texture 
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D, texNames[indicator], 0);
        
        //  set viewport to texture's size
        ofDisableSetupScreen();
        glViewport(0, 0, TEXWIDTH, TEXHEIGHT);
        
        //  clear display 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void endRenderingFbo(){
        //  end rendering to FBO
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        ofPopView();

    }
    
    GLuint count;
    int indicator;
    
    ofShader shader;
    bool doShader;
    
};



#endif
