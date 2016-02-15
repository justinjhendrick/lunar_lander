data = read.csv("pid.csv")

pdf(file="tune_pid.pdf")
matplot(data,
        main="Wrapped PID tuning for lunar_lander",
        ylab="value",
        xlab="frame number")
legend("bottomleft", c('desired (r)', 'current (r)', 'manip_var (r/s^2)'), col=1:4, pch=1)

dev.off()
