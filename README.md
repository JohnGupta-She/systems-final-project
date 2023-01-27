# systems-final-project

# MODCHECK

# Developers: John Gupta-She (honorary Josiah Moltz)
### Period 4
---
## High Level Description:
CHAAAAATROOOOOOOM(s, 10 to be exact)!!!


## User Interface:
- make server
- make client -> type name, enter rooms as wanted!! MENSAGE CON SUS AMIGOS!!! /exit to leave the room and go back to the LOBBY!!

---

## Technical Design:
- There is a big momma server, big momma has lots of children!(forking server deluxe):
- Room subservers; deez are the chatrooms. they have a list of clients who are "in" the chatroom at the moment. ∃ a well known pipe into each of da rooms., this is how the subservers talk to them.
- Client subservers; deez are attached to clients via socket. Each client has its own subserver, each subserver has its own well known pipe leading into it which is how each room talks with it. There is a location variable to tell it which room its in (which pipe to send messages)
- ALL ROOMS AND CLIENT SUBSERVERS CAN TALK TO EACH OTHER VIA WKP AT ANY TIME, THEY CHOOSE WHO TO TALK TO BY THE LIST OF MEMBERS AND LOCATION VARIABLE RESPECTIVELY.


## Work Distribution:
John does it, Josiah morally supports

## Algorithms/Data Structures:
- there is a commonly used struct that sends the main data we want: the message sent, the name of the person who sent it, the id (an int assigned to the client upon joining used for identification)
- selects are used mucho para separar the incoming things.
---
## Timeline:
#### 1/24:
- do the one chatroom (did not hit this timeline lmao)
#### 1/25:
- finish it (LMAO!)
#### 1/26:
- stay up until 12 to finish it before senior grades are due (hit this one! soory aboot dat)
