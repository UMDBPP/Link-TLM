close all

plot((W3EAX_11.time - W3EAX_11.time(1))*86400/60,W3EAX_11.alt)
hold on
plot((KB3ZZI_9.time - KB3ZZI_9.time(1))*86400/60,KB3ZZI_9.alt)
grid on
ylabel('Altitude [m]')
xlabel('Time [min]')