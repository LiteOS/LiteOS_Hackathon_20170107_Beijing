export const ADD_GIFTS = "ADD_GIFTS";
export const SUB_GIFTS = "SUB_GIFTS";
export const ADD_TICKETS = "ADD_TICKETS";
export const CHANGE_NAME = "CHANGE_NAME";

export function addGift() {
    return {
        type:ADD_GIFTS
    }
}

export function addticket() {
    return {
        type: ADD_TICKETS
    }
}

export function subGift() {
    return {
        type:SUB_GIFTS
    }
}


export function changeName(newName) {
    return {
        type:CHANGE_NAME,
        newName
    }
}