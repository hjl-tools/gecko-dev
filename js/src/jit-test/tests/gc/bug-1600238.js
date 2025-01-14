// |jit-test| --enable-weak-refs

gczeal(0);
newGlobal();
nukeAllCCWs();
function f() {
    global = newGlobal({
        newCompartment: true
    });
    try {
        return global.eval("new FinalizationGroup(function(){})");
    } catch (e) {
        if (e instanceof TypeError && e.message.includes('dead')) {
            // Creating a new CCW to the global fails with
            // --more-compartments option.
            quit();
        }
        throw e;
    }
}
g = f();
g.register({}, {}, {});
startgc();
