CURSIVE -- a markdown parser
===

Foreward
---
The idea behind the cursive markdown parser was simple. I wasn't satisified with
the other implementations I've seen. So I decided I could do it better. I thought
it would be a project that I could bang out in a day. However, I knew that was 
a foolish thought. On the whole my development has been rather lackluster. An 
idea here, another there. A graveyard of half-done projects all fallen along the
way to scope creep or loss of interest. Sometimes a combination of both. This project
is supposed to be different. It'll be useful, something I want. Famous last words.

A promise
---

This project marks the first attempt of working through the problem in stages. Not
trying to tackle too much at once. Organization and progress throughout personal projects
has always plagued me. This marks my attempt to work through that. Documenting my steps,
working through it.

I promise to make an effort to commit to this project ~~daily~~ be it adding to the 
codebase, screwing things up, detailing the project or just keeping track of the ideas.


Documentation Procedure
---
A document ought to have a procedure to it. If it sucks, we'll rebuild it to not suck.
This is the first cut.

+ Write commit entries - Seems superflous doesn't it? Yah it is, but I doubt I'll write
the reason why I broke the entire project and checked it in right?
+ Title the categories - Obviously, so the stuff is understandable

My Requirements
---
- I want to create a parser that parses markdown syntax similar to githubs or Daring Fireballs
- I want to be able to use the parser to take notes, make journal entries or create blog posts

Supported Markdown
---

-- Headings
# H1                         (supported)
## H2                        (supported) 
### H3                       (supported)
#### H4                      (supported)
##### H5                     (supported) 
###### H6                    (supported)

Alternate H1                 (supported)
===
Alternate H2                 (supported)
---                         

-- Lists
+,- unordered list          (supported)
+,- nested unordered list   (supported)
1. ordered list             (not supported)
1. nested ordered list      (not supported)

-- Inline styles
*italics*                   (not supported)
*(bold)*                    (not supported)
_underline_                 (not supported)
![alt tag](img src)         (not supported)
[link text](link)           (not supported)


POT - Plan of Attack
---
I want to be able to do other things so I won't be dedicating a massive amount of time to this
project. But I want to be able to open source the code so others can comment, build-on, or use
the project.


