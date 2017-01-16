import {ADD_GIFTS,SUB_GIFTS,ADD_TICKETS} from "../action/actions"
var initial = {
    info: {
        username: "xxxx",
        age: "100",
        sex: "man"
    },
    wealth: {
        coins: 0,
        tickets: 0,
        gifts: 0
    }
};

function info(state = Object.assign({}, initial.info), action) {
    let newState = {};
    switch (action.type) {
        case "CHANGE_NAME":
            newState.username = action.newName;
            return newState;
        default:
            return state;
    }
}

function wealth(state = Object.assign({}, initial.wealth), action) {
    let newState = {};
    switch (action.type) {
        case ADD_GIFTS:
            newState.coins = state.coins - 1300;
            newState.gifts = state.gifts + 1;
            return newState;

        case SUB_GIFTS:
            newState.coins = state.coins + 1300;
            newState.gifts = state.gifts - 1;
            return newState;

        case ADD_TICKETS:
            newState.tickets = state.tickets + 1;
            return newState;
        default:
            return state;
    }
}
export const reducers = {
    info,
    wealth
};