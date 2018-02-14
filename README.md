# ProjectMORPG

The original idea for this project comes from the big MMORPG titles such as **World of Warcraft**. Basically, the aim is to build a solid foundation and structure for a later development. With the product of this assessment, the project has a basic networking framework to start implementing other gameplay related functionalities on top of it. 

Due to this, all the design and implementation decisions made in the progress were always made with the idea of building a **scalable**, **maintainable** and **modular** networking code in mind.

## Architecture

Regarding the architecture designed for this project, as I mentioned in the previous section, the idea was to have a solid structure that meets the needs of a multiplayer game that supports potentially **hundreds** or even **thousands** of clients simultaneously. Consequently, I chose a client-server architecture, with the clients performing the most part of the game logic, in order to relief the server from the responsibility of doing all of the calculations. However, the server will have the authority to manipulate the game state under certain conditions. For example, the server would check each player actions to detect any kind of cheating, like speed-hacks or players ignoring collisions. 

On top of that, due to this type of games not needing a real-time synchronization, and the gameplay is not based on reaction times and timing skills, the server will determine a fixed time step where every client agrees on what happens, as well as making all the calculations (such as predictions) based on these time stamps or ***ticks***. One clear example that I will discuss later that shows this behaviour is the spell casting, that basically consists in the client begins to cast certain spell at a given time stamp or tick, and then the server does the calculation of determining in which tick the casting will end, notifying every client.

## Protocols

As for the way that communication flows between the systems, there are two main types of messages: **notifications** and **updates**.

### Notifications

This type of network message represents any piece of information that needs to be processed for every client. Therefore, these messages are sent using the **TCP** connection that each game instance has with the server.

![Figure 1](/figures/figure_1.png)

Some examples of this kind of message are the initial conversation between client and server, where all the connection related parameters are set (client identification number, user name, etc.), and also some mechanics such as spell casting. 

In the figure we can see how the first message exchange takes place in the application. First, the client tries to establish a connection to the server. The server, that is waiting for incoming connections via a TCP socket, accepts the connection and creates a new socket to handle one part of the communication with that client (I will talk about the other part in the updates section). After accepting, the server assigns an identification number to the client, and sends it to the client, along with some server parameters such as the timestep and the tick the server is at that given moment.

On the other hand, the reason to include spells as notification instead of sending updates each tick with the remaining cast time is that every client can calculate this information every frame based on the time stamp that the casting began, and by sending only one message using TCP we reduce the number of messages sent through the network, as WoW-like spells usually have casting times of seconds.

### Updates

As the name suggests, updates are messages that become useless when a newest version is received. This is, every message regarding movement is an update, and is sent using UDP. In terms of implementation, the server uses one **UDP** socket for all the updates. With the number of message of this type that are sent and processed every tick, there is no need to have some TCP features like ACKs or package ordering during that interaction. What we win in this trade is a reduction of the network usage, factor that can be critical especially in server-side.

## Integration

In terms of integrating the networking code with the rest of the application, I will first describe the structure of the network system in both the server and the game client.

![Figure 2](/figures/figure_2.png)

### Structure 

For the server-side networking code, I built a **Network** and a **Client** class. The first one manages the server sockets, as well as storing all the clients connected at a given point in time. Those clients encapsulate all the data that the server has about them along with the TCP socket assigned. Inside the server there is only one instance of the network class, and it interacts with the Window in order to execute the needed operation according to the event fired.

In the same way, the game client also has a **Network system**, but it does not interact directly with the Windows Socket API. Instead, I implemented a small **NetFramework** that separates the game application from the WinSock, with the idea of modularity and portability in mind. This framework includes some basic socket classes, both TCP and UDP, as well as an address class and the definition of the *NetMessage* struct.

![Figure 3](/figures/figure_3.png)

### Integration

On the server-side, the networking code is **asynchronous event-based IO**, using the Windows events programming, with *WSAAsyncSelect* function. The reason for this decision is that as we discussed in class and after some research findings that supported that, the select function does not scale well for large numbers of clients, and one good alternative is changing the paradigm to event-driven programming.

For the client, the network code is integrated with the rest of the systems (Renderer, Input, etc.) using a non-blocking style. With that, the client will try to read any network message at the beginning of each game cycle (Update method in Game class) and send as many messages as possible each server tick without blocking.


##	Prediction and Lag Compensation

In this section I will cover the mechanisms used for making the experience smoother under poor network conditions. Specifically, there are two techniques implemented: **movement prediction** and **lag compensation**.

### Movement Prediction

Since the server dictates the pace of the updates with a fixed timestep, the clients only receive others’ positions in each tick, and in the time in-between the game application the game is being updated more than once. Therefore, if there isn’t any prediction technique, external clients would be moving in jumps each tick. Given the nature of the game, movement is not an essential part of the gameplay, so a linear model approach for predicting positions is suitable and gives a good sense of smooth movement.

![Figure 4](/figures/figure_4.png)

Looking at the diagram we can see that, since the game events are paired with the tick when they happen, we are able to calculate players’ future positions by using the last position received from outside, their velocity at that moment and the time passed since the last update. Consequently, the formula for calculating the player position looks like:

position = lastPosition + velocity * deltaTime

Where position is the calculated position for the present frame, *lastPosition* is the position received inside the last update. The *velocity* is calculated when a new position is received (using the new one and the *lastPosition* at that given moment), and *deltaTime* is the sum of the ticks from last update (in case of package drop) times the timestep, and the time from the last tick.

### Lag compensation

In terms of lag compensation techniques, the main decision in the project was to dictate the rhythm at which the updates are distributed. This way, every client can predict some events that will happen at a given tick in the future, as we saw in the last section about movement prediction. 

One practical example implemented in this project that benefits from this feature is, as I mentioned earlier, the spell casting.

![Figure 5](/figures/figure_5.png)

Just like the diagram shows, when a client begins to cast a spell, the server calculates the ending tick using the starting point and the spell data. Once it has that parameter, the server notifies each client of the incoming spell, providing the finishing tick. With this approach, the only part of the communication which can be negatively affected are the other clients, due to the fact that from their point of view, the casting takes a bit less. However, in the context of these game mechanics, casting a spell may take seconds, and therefore the difference between the perceived casting time and the real duration does not affect the overall experience. Moreover, in the case that the player that witnesses the casting from outside has a poor network connection, the idea of having missed part of the casting due to lag does not seem incoherent, as it is expected that the player’s connection is the responsible for that.

## Testing

For this section, I recorded a <a href="https://www.youtube.com/watch?v=r2j1hqF9evE" target="_blank">video</a> of some tests performed under certain **network conditions**, as it is the main source of information for the discussion. However, the tests shown are not the only ones that I did. I tried different combinations of conditions, as well as incorporating some debug messages in order to identify the root of some of the problems.

First, the application was tested with a server time step of 50 milliseconds, without any special network condition. This way, the movement prediction algorithm worked fine, calculating the positions between ticks completely fine, since the framerate is higher than the network updates. 

However, when lag came into play, the movement was starting to feel unrealistic at times. On one side, the direction changes happened instantaneously, correcting the position teleporting the player, and on the other, when testing high levels of latency (~300ms and above), some jumps started to happen. On the bright part, when testing the movement prediction with package drop, this last problem did not appear, not even losing half of the data.

Apart from these two main network conditions (latency and package drop), the prediction technique was also tested forcing packages to arrive out of order and/or duplicated, but none of those affected the results, thanks to the messages being marked with the tick when they were emitted.

Regarding the lag compensation mechanism, implemented in form of spell casting, it is fair to say that it performed as expected, and did not suffer from any of the poor network conditions. As we can see in the demonstration video, all the spells, from 1 to 3 seconds, finish casting simultaneously. 

On one hand, forcing some packages to drop was not an issue thanks to the communication using TCP. On the other hand, latency had the effect mentioned in the Lag compensation section, shortening the casting time perceived by the observer player with lag.

## Potential Solutions and Improvements

For the closing section for the document, I would like to describe some changes or additions that may improve the performance and polish the feeling of the current implementation.

 - Integrating **interpolation** with the current prediction technique can solve the issue of players teleporting when guessing fails. This way, when one changes the movement direction, other clients with some latency trace a viable path between the guessed point and the actual position, correcting the path in a more realistic way. 
 

   In general, all the tests showed that the movement prediction algorithm has room for improvement.

 - For the spell casting mechanic, there are some ways that it can be upgraded. One of them could be calculating the ending tick using the one when the server receives the cast, instead of the original client tick. Doing this we remove the impact of the latency of the player casting the spell. The consequences are that the player doing the action is affected by his lag, which seems normal, and other players are not penalized by others’ latency.
 
   Also, for the spell mechanic, in terms of the visuals and more specifically the casting animation, one feature that could help with the “reduced perceived duration” is having an anticipation part. In order words, the last part of the cast being different and easy to identify, so players that could have missed part of the time at first can predict when the spell is ending. 


# Read More

If you want to have a look at the code or perhaps read the final report, you can do so using the links down below. Also, for more info don't hesitate to <a href="/about.html">contact</a> me.
