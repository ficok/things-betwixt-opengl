#ifndef THINGSBETWIXT_FRAMEBUFFER_H
#define THINGSBETWIXT_FRAMEBUFFER_H

#include <glad/glad.h>
#include <utils.h>
#include <errors.h>

#define MAX_COLOR_BUFFERS 10

enum ImageType
{
    RGB = 0,
    RGBA = 1
};

class Framebuffer
{
private:
    unsigned int _fbo;
    unsigned int _rbo;
    unsigned int _colorBuffer;
    int nrColorBuffers;
    bool deleteRenderbuffer = false;
public:
    unsigned int colorBuffers[MAX_COLOR_BUFFERS];

    Framebuffer(ImageType type, int nrColorBuffers, bool depth, bool stencil)
    {
        this->nrColorBuffers = nrColorBuffers;
        // generate and bind this framebuffer
        glGenFramebuffers(1, &_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        /*
         * why did I use _colorBuffer for a single color buffer, and colorBuffers[] for multiple?
         * - it didn't work any other way.
         */
        // create a texture which will serve as a color buffer
        nrColorBuffers == 1 ?
        glGenTextures(1, &_colorBuffer) : glGenTextures(nrColorBuffers, colorBuffers);

        for (unsigned int i = 0; i < nrColorBuffers; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, nrColorBuffers == 1 ? _colorBuffer : colorBuffers[i]);

            switch (type) {
                case RGB:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, S_WIDTH, S_HEIGHT,
                                 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                case RGBA:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, S_WIDTH, S_HEIGHT,
                                 0, GL_RGBA, GL_FLOAT, nullptr);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                   nrColorBuffers == 1 ? _colorBuffer : colorBuffers[i], 0);
        }

        if (nrColorBuffers == 1)
            colorBuffers[0] = _colorBuffer;

        // generate and configure a render buffer object for depth and stencil
        if (depth)
        {
            deleteRenderbuffer = true;
            if (stencil)
            {
                glGenRenderbuffers(1, &_rbo);
                glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, S_WIDTH, S_HEIGHT);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
            }
            else
            {
                glGenRenderbuffers(1, &_rbo);
                glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, S_WIDTH, S_HEIGHT);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo);
            }
        }

        // tell opengl which attachments we will use for rendering
        if (nrColorBuffers > 1)
        {
            unsigned attachments[nrColorBuffers];
            for (int i = 0; i < nrColorBuffers; ++i)
                attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            glDrawBuffers(nrColorBuffers, attachments);
        }

        // check the status of the newly created framebuffer
        assert((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE),
               "ERROR [framebuffer]: framebuffer creation failed.\n");

        // unbind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void activate()
    const {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    }

    void deactivate()
    const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void del()
    const {
        if (nrColorBuffers == 1)
            glDeleteTextures(1, &_colorBuffer);
        else
            glDeleteTextures(nrColorBuffers, colorBuffers);
        if (deleteRenderbuffer)
        glDeleteRenderbuffers(1, &_rbo);
        glDeleteFramebuffers(1, &_fbo);
    }
};

#endif //THINGSBETWIXT_FRAMEBUFFER_H