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
```
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
```

POT - Plan of Attack
---
I want to be able to do other things so I won't be dedicating a massive amount of time to this
project. But I want to be able to open source the code so others can comment, build-on, or use
the project.


HLD - High Level Design
---

What is HTML? HTML is a markup language that is derived from XML. HTML
is used to tell webbrowsers how to render a page on the screen.

HTML consists of tags which are used to instruct the browser how to render
a page. 

The layout of a common HTML is as follows
```
<html>
	<head><style><!-- put your styles in here --></style></head>
	<body>
		<h1>Example Header</h1>
		<p>Sample paragraph goes here</p>
	</body>
</html>
```

The way I plan on parsing the mark down is to translate the structures in
markdown into C++ objects representing the appropriate HTML. Each piece will
contain the content and will be built similar to a DOM or Linked List so we can just print out the pieces afterward.  I don't know how 
performant this will be, I can't say I care right now. Make it work. Make it
good. Make it fast.

Goof Log
---
{08.29.2014} - ~~I goofed and broke this whole thing. I'll fix it. I will.~~ Fixed it 


Log
---
08.13.2014 
- Completed initial logging
- Completed first cut parsing for headings
- Debugged printing document

08.15.2014
- Completed statemachine beginning
- Reworked parsing engine back to where it was

08.18.2014
- Fixed state machine to use only one state instance per state transition
- owned_ptr implementation

08.29.2014
- Started Windows project (which doesn't compile in 2010). Woops
- Pushed to GitHub
- Implmented inlining of node content

ToDo
---
- {08.15.2014} Practical nesting of nodes i.e. how the fk?
- {08.15.2014} Implement paragraphs
- Upgrade logging to actually be more useful than just "info"
- Start looking at exception support
- Tighten up the test framework; so it doesnt just return use exceptions
- {08.18.2014} Change this to a state machine
   - Read first line
   - Either it matches a case or we put it in waiting
   - Read next line either it matches an end case or we append
   - Repeat
- {08.23.2014} Implement nested lists 
   - on_enter_state - push a new list on the stack
   - if the line is within the same level add it to the list
   - else transition to the same state
   - drop a level and print on-exit 
- {08.29.2014} Implement inline stuff on the nodes
- Implement inline parsing
- Change the entire node structure to just be content nodes
- Refactor to support HTML Document AND PDF Document
- Spell check
- CSS stuff
