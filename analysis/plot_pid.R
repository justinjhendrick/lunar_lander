data = read.csv("pid.csv")

pdf(file="tune_pid.pdf")
matplot(data,
        main="PID tuning for point_retrograde()",
        ylab="value (radians or radians/second^2)",
        xlab="time (milliseconds)",
        pch=1)
abline(h=0, lty=2)
legend("bottom",
       c('desired orientation',
         'current orientation',
         'manipulated (r/s^2)'),
       col=1:3,
       pch=1)

dev.off()
