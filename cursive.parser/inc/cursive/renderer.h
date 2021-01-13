#ifndef __cursive_renderer_h__
#define __cursive_renderer_h__

#include <cursive/document.h>

namespace cursive
{

    /// <summary>
    /// The rendering processor iterates the Document Syntax Tree (DST) and uses the renderer
    /// to output to a stream;
    /// </summary>

    template <typename CharT, typename RendererT>
    void render(const basic_document<CharT>& doc, std::ostream& os)
    {
        RendererT::render(doc, os);
    }

}

#endif // __cursive_renderer_h__
