#ifndef __document_h__
#define __document_h__

#include <cursive/element.h>

namespace cursive
{
    template <typename CharT>
    class basic_document
    {

    public:
        using char_t = CharT;
        using element_t = basic_element<char_t>;
      
    public:
        basic_document()
            :root_(element_types::document)
        {
        }
        const element_t& child_at(size_t index) const
        {
            return root_.child_at(index);
        }
        const element_t& operator[](size_t index) const
        {
            return root_[index];
        }
        element_t& add(element_t&& child)
        {
            return root_.add(std::move(child));
        }

        element_t& add(const element_t& child)
        {
            return root_.add(child);
        }
        
        const std::vector<element_t>& children() const
        {
            return root_.children();
        }

        typename element_t::const_iterator begin() const
        {
            return root_.begin();
        }
        
        typename element_t::const_iterator end() const
        {
            return root_.end();
        }

    private:
        element_t root_;
    };

    using document = basic_document<char>;
    using wdocument = basic_document<wchar_t>;


    template <typename CharT>
    basic_document<CharT> make_document()
    {
        return basic_document<CharT>();
    }
}

#endif // __document_h__